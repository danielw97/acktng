# Design Proposal: Accurate Executable Snapshot Through Hotreboot

## Problem

When the server crashes and dumps a core, the startup script saves the core
alongside a snapshot of `src/ack`. The snapshot is taken once, immediately
before each outer process launch. This is correct for a straight crash, but
wrong after a hotreboot:

1. Startup snapshots `src/ack` → `cores/ack.XXXXXX`
2. Operator rebuilds `src/ack` (new binary on disk)
3. `do_hotreboot()` calls `execl()` — the running process is replaced with the
   new binary
4. The new binary crashes → core dump reflects the new binary
5. The startup script finds the core and pairs it with the stale pre-run
   snapshot, not the binary that was actually running

The mismatch makes the core useless for debugging: gdb will report mismatched
symbols or silently produce wrong stack traces.

---

## Approach

Before `do_hotreboot()` calls `execl()`, snapshot the binary that is about to
be exec'd into a well-known file in `../cores/`. The startup script already
watches that directory; it just needs to prefer this fresh snapshot over the
one it took at process start.

### Snapshot file convention

The hotreboot snapshot is written to `../cores/ack.hotreboot` (relative to the
working directory `area/`, so the absolute path is `cores/ack.hotreboot`). The
startup script, when collecting a core after exit, checks whether
`../cores/ack.hotreboot` exists and uses it in preference to the original
`ack_snapshot` tempfile, then removes both.

A fixed name (rather than a tempfile) is intentional: only one hotreboot can be
in flight at a time, and using a fixed name avoids accumulating orphaned files
when the operator kills the server mid-hotreboot.

### C changes (`src/hotreboot.c`)

Add a helper that copies the about-to-be-exec'd binary to
`../cores/ack.hotreboot` just before the `execl()` calls in `do_hotreboot()`.
The path of the binary is already resolved via `readlink("/proc/self/exe")` a
few lines earlier; that same resolved path is what we copy.

```c
static void snapshot_exe_for_coredump(const char *exe_path)
{
    /* Copy exe_path → ../cores/ack.hotreboot so the startup script can pair
     * it with any core produced by the new process image. Failures are
     * logged but do not abort the hotreboot. */
    char buf[4096];
    int src_fd, dst_fd;
    ssize_t nr, nw;

    src_fd = open(exe_path, O_RDONLY);
    if (src_fd < 0) {
        log_f("snapshot_exe_for_coredump: open(%s): %m", exe_path);
        return;
    }
    /* O_TRUNC so a stale file from a previous hotreboot is replaced */
    dst_fd = open("../cores/ack.hotreboot", O_WRONLY | O_CREAT | O_TRUNC, 0755);
    if (dst_fd < 0) {
        log_f("snapshot_exe_for_coredump: open ../cores/ack.hotreboot: %m");
        close(src_fd);
        return;
    }
    while ((nr = read(src_fd, buf, sizeof(buf))) > 0) {
        nw = write(dst_fd, buf, (size_t)nr);
        if (nw != nr) {
            log_f("snapshot_exe_for_coredump: short write");
            break;
        }
    }
    close(src_fd);
    close(dst_fd);
    log_f("snapshot_exe_for_coredump: saved %s → ../cores/ack.hotreboot", exe_path);
}
```

Called in `do_hotreboot()` after `exe_path` is resolved and before either
`execl()` call:

```c
    snapshot_exe_for_coredump(exe_path);   /* <-- add this line */
    execl(exe_path, exe_path, buf, "HOTreboot", buf2, buf3, (char *)NULL);
    ...
    execl(EXE_FILE, EXE_FILE, buf, "HOTreboot", buf2, buf3, (char *)NULL);
```

The fallback `execl(EXE_FILE, ...)` path does not have a resolved exe path
readily available, but `EXE_FILE` is a compile-time constant path to the
binary. We call `snapshot_exe_for_coredump(EXE_FILE)` before that fallback as
well.

### Shell changes (`scripts/startup`)

After the server exits, prefer `../cores/ack.hotreboot` over the original
snapshot:

```sh
  # If a hotreboot snapshot exists, it reflects the last exec'd binary;
  # use it in place of the startup-time snapshot.
  if [ -f ../cores/ack.hotreboot ]; then
    rm -f "$ack_snapshot"
    ack_snapshot=../cores/ack.hotreboot
  fi

  for corefile in core core.*; do
    if [ -f "$corefile" ]; then
      coredir=../cores/$(date +%Y%m%d_%H%M%S)_$$
      mkdir -p "$coredir"
      mv "$corefile" "$coredir/"
      mv "$ack_snapshot" "$coredir/ack"
      ack_snapshot=""
      echo "startup: core dump saved to $coredir" >>$logfile
    fi
  done

  rm -f "$ack_snapshot" ../cores/ack.hotreboot
```

The final `rm -f` cleans up both the startup snapshot and the hotreboot
snapshot regardless of whether a core was found.

---

## Affected Files

| File | Change |
|------|--------|
| `src/hotreboot.c` | Add `snapshot_exe_for_coredump()` helper; call it before both `execl()` calls in `do_hotreboot()` |
| `scripts/startup` | Prefer `../cores/ack.hotreboot` over the startup-time snapshot when collecting cores |

No other files are affected. No new headers, no changes to `ack.h` or
`typedefs.h`.

---

## Trade-offs and Notes

**What if the `../cores/` directory doesn't exist yet?**
The startup script now creates it unconditionally with `mkdir -p ../cores`
before the loop. `snapshot_exe_for_coredump` logs and returns harmlessly if the
`open()` fails, so a missing directory doesn't abort the hotreboot.

**What if hotreboot fails after the snapshot but before exec?**
The hotreboot snapshot file sits in `../cores/ack.hotreboot` and is cleaned up
at the end of the next startup loop iteration (`rm -f ...
../cores/ack.hotreboot`). It won't pollute future crashes.

**Multiple hotreboots before a crash?**
Each hotreboot overwrites `../cores/ack.hotreboot` (`O_TRUNC`), so only the
most recent hotreboot binary is kept — which is correct, since that's the image
that would have crashed.

**Binary size / I/O cost**
Copying the binary on every hotreboot adds a brief I/O burst proportional to
the binary size (typically a few MB). This happens at hotreboot time when
players are already waiting 30 seconds, so it is not a concern.

**Unit test coverage**
`snapshot_exe_for_coredump` is a file-copy utility with straightforward
failure modes. A unit test would require mocking `open`/`read`/`write`, which
adds more test scaffolding than the function warrants. The function will be
verified by the integration test (boot + hotreboot path) once hotreboot is
exercised in CI — that is a future improvement outside the scope of this
proposal.
