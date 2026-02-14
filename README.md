# Machine-Learning-with-C

A small, self-contained C project that demonstrates a simple machine-learning pipeline (linear regression via gradient descent) for predicting housing prices.
Key features include CSV data loading with optional one-hot encoding, min-max normalization, training with gradient descent (interruptible), MSE/MAPE evaluation, plotting (using **gnuplot**), and a small interactive CLI for exploring the dataset and making custom predictions.

---

## Features

* Load CSV datasets with optional **one-hot encoding** for categorical columns. 
* Add a user-defined custom feature computed from row values (the example `custom_formula` combines income, rooms and a location flag). 
* Train a linear regression model with **gradient descent** and configurable learning rate & epochs. Training periodically reports MSE and can be stopped by pressing `S`. 
* Min-max normalization for features (train set) and conversion of test set using train normalization parameters. 
* Plot actual vs predicted values and feature vs price plots using **gnuplot** (non-Windows builds). 
* Interactive menu for: plotting, splitting train/test, training model, checking model performance, custom predictions, toggling the custom feature, and exit. 

---

## Requirements

* Linux (or POSIX-like) recommended for plotting (`gnuplot` integration). Plotting code is guarded with `#ifndef _WIN32`. 
* `gcc` (or another C compiler) and `make`.
* `gnuplot` installed for interactive plots.
* A terminal that supports reading keystrokes for stopping training.

---

## Build & Run

This project includes a `Makefile` and supports the following commands (as provided in the original project):

```bash
# Build using make
make

# Run the compiled program
make run

# Compile and run in one command
make compile-run
```

(These commands are the project defaults.) 

---

## Typical Usage / CLI

When you run the program, you will see an interactive menu similar to:

```
1. Plot points to check features versus the target and their correlation in dataset
2. Make and load the train and test dataset
3. Find weights and produce the model with gradient descent - linear regression
4. Check the model performance and precision
5. Custom home price prediction
6. Add\Remove custom feature to main dataset
7. Exit
```

Select the option number to perform each action. The program asks for parameters when necessary (for example, train ratio for option 2 or learning rate & epochs for option 3). 

### Training

* When choosing to train the model you will be prompted for a learning rate and epoch count.
* The training loop reports MSE every 100 epochs.
* To stop a long training run, press `S` (or `s`) — training recognizes that keypress and stops gracefully. 

### Plotting

* The program can plot individual features vs. price and also show an Actual vs Predicted scatter plot with a `y = x` line. Plotting uses `gnuplot` and will wait for you to close the plot window before continuing. 

### Custom Prediction

* After training a model you can input feature values for a single row and get a predicted price. The program normalizes your inputs using training normalization parameters and applies the learned weights & bias to compute the prediction. 

---

## Data & Preprocessing

* Example dataset filename used in the code: `housingprices.csv`. The code sets the price (target) column at index `8`. You can adapt these values to your dataset. 
* One-hot encoding (OHE) support: you can declare which CSV columns are categorical and provide the categorical values to expand into multiple feature columns during CSV parsing. 
* Min-max normalization (train set is normalized and normalization parameters are recorded so test & custom inputs are transformed in the same way). 

---

## Code Layout (high-level)

* `source/main.c` — CLI and overall flow (load data, split, train, evaluate, custom prediction). 
* `header/struct.h`, `source/struct.c` — basic `Dataset` and `Weights` structs and helpers (constructor/destructor). 
* `header/input_utils.h`, `source/input_utils.c` — CSV parsing, OHE handling, and basic terminal utilities (flush, key handling).
* `header/preprocess.h`, `source/preprocess.c` — normalization, train/test split, custom feature addition, and basic analysis utilities.
* `header/train.h`, `source/train.c` — gradient descent training, prediction function, and MSE/MAPE calculation.
* `header/plot_utils.h`, `source/plot_utils.c` — plotting helpers that invoke `gnuplot` for visualization.

---

## Notes & Tips

* The project uses an in-memory `Dataset` structure where `data[row][col]` are `double`s. Memory is allocated per row — be mindful of very large datasets. 
* The sample `custom_formula` function demonstrates how to compute a derived feature from existing columns. You can replace it with your own formula to explore feature engineering effects. 
* Plotting is disabled on Windows builds in the source; however, the rest of the functionality should still work on Windows if you adapt the plotting code or omit plots. 

---

## Contributing

If you want to extend the project:

* Add more advanced optimizers (e.g., momentum, RMSprop).
* Implement regularization (L1/L2) in `train.c`.
* Improve error handling and CSV parsing corner cases.
* Add unit tests for preprocess, train, and input utilities.

---

## License & Contact

This repository is provided as-is for learning / demonstration purposes. If you want me to prepare a `LICENSE` file or to polish any part of this README (shorten, expand examples, add diagrams, or include command outputs/screenshots), tell me what format you prefer and I’ll prepare it.

---

### Quick references

* Build & run: `make`, `make run`, `make compile-run`. 
* Main interactive menu & features: `source/main.c`. 
* Training & early stop logic: `source/train.c`. 

---

If you’d like, I can:

* Produce a shorter “Quick Start” README with only the essentials, or
* Produce a `README.md` file content ready to paste into your repo (I can paste the full Markdown here). Which do you prefer?
