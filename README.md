# AMDBugreportLodBias
Repro case for AMD bug report https://community.amd.com/thread/244097

## How to use
1. Open solution in Visual Studio (2019)
2. Compile and run
3. Press 1 and 2 to switch between plain textured view and mipmap level visualization
4. Press SPACE to save current viewport as a screenshot in the project directory

Directory "results" contains screenshots and diffs for those screenshots made using different AMD GPUs, see file names.

## Bug description

Having LOD bias set to greater than 0 leads to significant texture sampling differences on AMD RX 5700XT compared to other AMD GPUs (tested R9 200, FirePro W7000, Radeon Pro WX9100 and other).

### Expected

No significant differences in LOD selection and texture sampling results between RX 5700XT and other AMD GPUs when LOD bias is enabled for a texture.

### Actual

RX 5700 XT samples textures somehow different compared to other AMD GPUs. This leads to e.g. failed tests for our software. At some points differences are also clear visible to the human eye.
See images and diffs in the results directory.
