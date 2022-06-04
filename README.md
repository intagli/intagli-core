# Intagli 
## Build

### Requirements

- meson build
- wayland


## Compile and generate `compile_commands.json`

Requirements: bear

```
# Setup Build Dir
meson setup out
bear -- meson compile -C ./out
```

