# ECE 161B — Lab 2: Echo Effects & Notch Filter Design

**Course:** ECE 161B — Digital Signal Processing  
**Author:** Christian Alonzo  
**Institution:** UC San Diego, Jacobs School of Engineering

---

## Overview

This lab covers the design and implementation of audio DSP operations in both MATLAB and C. The core topics are:

- Echo effect implementation
- FIR notch filter design (derived from a lowpass filter)
- IIR notch filter design using pole-zero placement
- Porting MATLAB implementations to C

---

## Repository Structure

```
lab2/
├── matlab/
│   ├── task1_echo.m            # Echo effect (Task 1)
│   ├── task2_interference.m    # Sinusoidal interference + spectrograms (Task 2)
│   ├── task3_fir_design.m      # FIR notch filter coefficients (Task 3)
│   ├── task4_fir_filter.m      # fir_filter() function + filtering (Task 4)
│   ├── task5_iir_filter.m      # IIR notch filter design + filtering (Task 5)
│   └── generate_sigs.m         # Helper from Lab 0
├── c/
│   ├── task6_echo.c            # Echo effect in C (Task 6)
│   ├── task7_fir_notch.c       # FIR notch filter in C (Task 7)
│   └── task8_iir_notch.c       # IIR notch filter in C (Task 8)
├── audio/
│   ├── speech.wav              # Input audio file
│   ├── speech_echo.wav         # Output: echo version (Task 6)
│   ├── speech_fir_filtered.wav # Output: FIR filtered (Task 7)
│   └── speech_iir_filtered.wav # Output: IIR filtered (Task 8)
└── README.md
```

---

## Tasks Summary

### Task 1 — Echo Effect (MATLAB)
Applies an echo to `speech.wav` using:

```
y[n] = 0.7·x[n] + 0.3·x[n - Ne]
```

where `Ne` corresponds to a **0.5s delay** at Fs = 48 kHz (Ne = 24,000 samples).

---

### Task 2 — Sinusoidal Interference (MATLAB)
Adds a 2.4 kHz sinusoidal tone to the speech signal:

```
w[n] = x[n] + v[n]
```

Spectrograms of both `x` and `w` are generated to visualize the added narrowband interference.

---

### Task 3 — FIR Notch Filter Design (MATLAB)
A **14th order FIR notch filter** with a notch at **2.4 kHz** is derived from an ideal lowpass filter. The magnitude response is verified using `freqz`.

**Design approach:** A notch filter is obtained by subtracting a bandpass filter (centered at the interference frequency) from an allpass. The bandpass is constructed by modulating a lowpass prototype.

---

### Task 4 — FIR Filtering (MATLAB)
`fir_filter(w, h)` applies the FIR coefficients to the noisy signal `w`. The spectrogram of the filtered output `s` is compared against the noisy input to verify interference removal.

---

### Task 5 — IIR Notch Filter (MATLAB)
A **2nd order IIR notch filter** is designed by placing:
- **Zeros** on the unit circle at ±ω₀ (where ω₀ = 2π × 2400/48000)
- **Poles** just inside the unit circle at the same angles to maintain a narrow notch

The `iir_filter(x, a, b)` function from Lab 1 is reused to filter `w` and produce `m`. A spectrogram and listening comparison are included in the report.

---

### Tasks 6–8 — C Implementations
Each MATLAB task is ported to C:

| Task | File | Description |
|------|------|-------------|
| 6 | `task6_echo.c` | Echo effect |
| 7 | `task7_fir_notch.c` | FIR notch filter |
| 8 | `task8_iir_notch.c` | IIR notch filter |

Each C program reads `speech.wav` (or the noisy version), processes the audio, and writes the output to a new `.wav` file.

---

## Parameters

| Parameter | Value |
|-----------|-------|
| Sample Rate (Fs) | 48,000 Hz |
| Echo Delay (Ne) | 24,000 samples (0.5s) |
| Interference Frequency | 2,400 Hz |
| FIR Filter Order | 14 |
| IIR Filter Order | 2 |

---

## Dependencies

**MATLAB**
- Signal Processing Toolbox (`freqz`, `spectrogram`)

**C**
- Standard C libraries (`stdio.h`, `stdlib.h`, `math.h`)
- A WAV I/O library (e.g., `libsndfile`) or custom WAV read/write helpers from Lab 0

---

## How to Run

**MATLAB:** Open and run each `.m` script in order (Task 1 → Task 5). Ensure `speech.wav` is in the working directory.

**C:**
```bash
gcc task6_echo.c -o echo -lm
./echo

gcc task7_fir_notch.c -o fir_notch -lm
./fir_notch

gcc task8_iir_notch.c -o iir_notch -lm
./iir_notch
```

---

## Notes
- All signals are min-max normalized to the range [-1, 1] before processing.
- Report is written in LaTeX using the NeurIPS 2015 template for extra credit eligibility.
