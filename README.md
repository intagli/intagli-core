# Intagli 
## Build

### Requirements

Intagli requires Vulkan support on your graphics card.

Intagli requires Vulkan SDK. Download [here](https://vulkan.lunarg.com/sdk/home).  

### Install dependencies

#### Arch linux
Run commands  
```
sudo pacman -Syu
sudo pacman -S clang python3 glfw-wayland
pip3 install --user meson
```

### Build with Meson
```
meson setup ./out/
cd out
meson compile
```


## Compile and generate `compile_commands.json`

Requirements: bear

```
# Setup Build Dir
meson setup out
bear -- meson compile -C ./out
```

