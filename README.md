# acktng 4.3.1 release 1
ACK!TNG 4.3.1 public Release 1

This is the current release of Release 1 from ackmud.com:8890. This code will be up to date with what is currently running there, including all area files, bugfixes, etc.

Note: No playerfiles will be included with this. If you want to boot this up, you must create a player directory with directories a-z

## Open PR validation workflow
A GitHub Actions workflow is included at `.github/workflows/validate-open-prs.yml`.

It runs on a schedule (hourly) and on manual dispatch. For each currently open pull request, it:
1. Enumerates open PRs through the GitHub API.
2. Checks out `refs/pull/<PR_NUMBER>/head`.
3. Runs `make unit-tests` from `src/` to build and execute the project's test suite.
4. If tests pass, submits an automated PR approval review.
5. If tests fail, submits an automated `REQUEST_CHANGES` review and fails the workflow job.

If no PRs are open, the workflow exits with a no-op status.

To enforce merge safety, configure branch protection so PRs require this workflow check to pass and require at least one approving review.
