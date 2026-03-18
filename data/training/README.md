# NPC LoRA Training Data

This folder contains ChatML `.jsonl` training data for NPC dialogue LoRA tuning,
aligned with `docs/proposals/ai.md` section 10.

## Files

- `npc_training.jsonl`: Training examples (one JSON object per line).

## Generating / Regenerating Data

Use `generate_npc_training_data.py` at the repo root:

```sh
# Requires anthropic package and API key
pip install anthropic
export ANTHROPIC_API_KEY=your-key-here

# Generate 1500 examples (default) into data/training/npc_training.jsonl
python3 generate_npc_training_data.py

# Custom count or output path
python3 generate_npc_training_data.py --count 1500 --out data/training/npc_training.jsonl

# Dry run (no API calls, writes placeholders to verify structure)
python3 generate_npc_training_data.py --dry-run

# Use a specific model (default: claude-haiku-4-5-20251001)
python3 generate_npc_training_data.py --model claude-haiku-4-5-20251001

# Reproducible output with fixed seed (default: 42)
python3 generate_npc_training_data.py --seed 42
```

## Training

### Step 1 — Create npc_config.yml

```yaml
base_model: meta-llama/Meta-Llama-3.1-8B-Instruct
model_type: LlamaForCausalLM
tokenizer_type: AutoTokenizer

load_in_4bit: true

datasets:
  - path: npc_training.jsonl
    type: chat_template
chat_template: chatml

dataset_prepared_path: last_run_prepared
val_set_size: 0.05
output_dir: ./outputs/npc-model

adapter: qlora
lora_r: 16
lora_alpha: 16
lora_dropout: 0.05
lora_target_modules:
  - q_proj
  - k_proj
  - v_proj
  - o_proj
  - gate_proj
  - up_proj
  - down_proj

gradient_accumulation_steps: 4
micro_batch_size: 2
num_epochs: 3
optimizer: adamw_bnb_8bit
lr_scheduler: cosine
learning_rate: 0.0002

bf16: auto
gradient_checkpointing: true
warmup_steps: 10
evals_per_epoch: 4
saves_per_epoch: 1
```

Key notes:
- `adapter: qlora` + `load_in_4bit: true` = QLoRA (4-bit base + bf16 adapter)
- `lora_r: 16` / `lora_alpha: 16` → effective scale of 1.0 (conservative, appropriate for behavioral fine-tuning)
- `val_set_size: 0.05` holds back 5% of the JSONL for validation
- All 7 projection matrices are targeted — standard for instruction-tuned Llama models

### Step 2 — Install Axolotl with ROCm

The project uses an AMD RX 7900 XTX (RDNA3, gfx1100), so use PyTorch ROCm
builds — not CUDA:

```sh
pip install torch torchvision torchaudio \
    --index-url https://download.pytorch.org/whl/rocm6.1
pip install axolotl bitsandbytes
```

Verify ROCm is visible:

```sh
python -c "import torch; print(torch.cuda.is_available(), torch.version.hip)"
```

### Step 3 — Run training

```sh
axolotl train npc_config.yml
```

Expected: 30–90 minutes on the 7900 XTX for ~1,000 examples at 3 epochs.

Watch validation loss — with `evals_per_epoch: 4`, you get 12 eval checkpoints
total. Stop early if val loss plateaus or rises (overfitting risk on small
datasets).

### Step 4 — Export to GGUF for Ollama

```sh
# Merge the LoRA adapter back into base weights
python axolotl/scripts/merge_lora.py npc_config.yml

# Convert to GGUF with Q4_K_M quantization
python llama.cpp/convert_hf_to_gguf.py outputs/npc-model \
    --outfile npc-model.Q4_K_M.gguf \
    --outtype q4_k_m
```

### Step 5 — Serve via Ollama

Create a Modelfile:

```
FROM ./npc-model.Q4_K_M.gguf
PARAMETER num_ctx 8192
PARAMETER temperature 0.7
```

Register and serve:

```sh
ollama create ack-npc -f Modelfile
ollama serve
```

The game server's `call_openclaw()` needs no changes — the fine-tuned model is
a drop-in at the same OpenAI-compatible endpoint (`localhost:11434`).

## Iteration Workflow

As the server runs, bad NPC responses from play logs are the most valuable
training signal. Per iteration cycle:

1. Collect bad NPC responses from play logs
2. Write corrected versions (in-character, concise, correct register)
3. Append corrections to `npc_training.jsonl`
4. Re-run `axolotl train npc_config.yml`
5. Re-export to GGUF and redeploy via Ollama

## Dataset Spec

- **Target size:** 1500 examples
- **Matrix:** 5 cities × 10 races × 5 archetypes × 4 scenarios = 1000 base cells,
  cycled to 1500 with shuffled variety
- **Multi-turn:** ~25% of examples (≈375) are 2-turn conversations
- **Format:** ChatML `.jsonl` — one JSON object per line with a `messages` array

## System Prompt Format

Every system prompt includes:
- Explicit conditioning tags: `[CITY_ACCENT:<city>]` and `[RACIAL_INCLINATION:<race>]`
- Both section-10 style directives:
  - `Speak with a <City> accent: ...`
  - `Your <Race> nature colors your speech: ...`

## City Accents

| City | Speech Style |
|------|-------------|
| Midgaard | Clipped bureaucratic phrasing, ledger idiom |
| Kowloon | Terse, elliptic, proverb-laden |
| Mafdet | Formal titles, trade jargon, oath references |
| Kiess | Measured, watchful phrasing, frontier caution |
| Rakuen | Warm but weary, garden metaphors, communal |

## Racial Speech Inclinations

Covers all 10 races: Human, Khenari, Khephari, Ashborn, Umbral, Rivennid,
Deltari, Ushabti, Serathi, Kethari.

## Archetypes

City guard, innkeeper, temple clerk, harbor official, road warden.

## Scenario Types

1. `practical_advice` — traveler asks for useful local information
2. `local_knowledge` — traveler asks about history, culture, or places
3. `immersion_break` — player asks meta/out-of-character question (NPC deflects in-character)
4. `hostile_or_tricky` — player is rude, aggressive, or tries to manipulate
