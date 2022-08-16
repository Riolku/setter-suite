# Setter Suite

A collection of tools for problem-setting, especially for use with [DMOJ](https://dmoj.ca), which you can also see [on github](https://github.com/DMOJ/judge-server).

## config.yml

`config.yml` is the file that this utility will check for many things. It is intended as a configuration for the problem. Keys include:

- `reference_sol`: The reference solution.
- `checker`: The checker, either a builtin name (only `identical` is supported as of now), a C++ file, or a python file, which will be treated as a raw checker, NOT a bridged one.
- `generator`: The generator, a file name.
- `generator_type`: The generator type, either `single` (the default), or `double`. See the `generate` script for an explanation of the differences.
- `case_counts`: an array of the number of cases in each suite.
- `case_points`: an array, of the same length as `case_counts`, indicating the points for each suite.
- `init_type`: the type of init.yml to generate. See `init-yml` for more details.
- `random_gen`: A generator to generate random test cases
- `validator`: A file to validate test cases. It should take the case as input, and return `0` on a valid case.
- `output_limit_length`: corresponds to the init.yml key that limits the output character count.
- `dependencies`: An array of batch dependencies. It is a list, one list per batch, each of which contains integers indicating the dependencies of the batch.

## Scripts

All the scripts should be invoked by first running `main.py`. Do not expect the scripts to work otherwise. On the command line, provide the command you wish to run, and any of its required arguments:

- `setter-build`: creates a `build/` folder. If you have a custom checker, it gets copied here. `init.yml` is copied here. If your init uses a zip file, the zip file will also copy here. Your generator will be copied here. Essentially, this folder contains everything you would upload to OneDrive.
- `countercase`: A fast-slow type script. Provide the file as the argument. It will run the `random_gen`, validate the input with the `validator`, run the reference solution and the provided solution, and then use the `checker` to determine if the solution is correct. If not, it will output the case. Note: this script uses every CPU core in order to be as efficient as possible. This may not be desired. You can specify a `workers` key in your `config.yml` or pass `--workers x` to change the number of cores used. Note however that using every CPU core does make this script very fast.
- `generate`: Generates data. If `generator_type` is `double`, it runs the generator to get input, and runs the reference solution for the corresponding output. `data/` is created, and contains the files in the form `<suite>.<case>.(in|out)`. A zip file is also created. If `generator_type` is `single`, it simply runs the generator for input and output.
- `init-template`: Creates a template. Specify the name of C++ files you want to include (without the `.cpp` extension), and it will output (to stdout), a concatenation of those files and all of their requirements. See also the `C++ Utilities` section.
- `init-yml`: Creates the `init.yml` file. If `init_type` is `zip`, it creates one to work with a zip file. If it is `generator`, then `generator_type` **must** be `single`, and it will create a init file using the generator.
- `invoke`: Custom invocation. Provide the name of the program as an argument, and provide to `stdin` a case. The case will be validated, the reference solution will wrong, as will the program provided, and the checker will be used to check. A detailed summary will be given.
- `judge`: Judge a file. This will run the program on _all_ test cases. Helpful for knowing which cases break a solution.
- `validate`: Validate the test data set against the validator.

## C++ Quickstart

For convenience, pre-built code is kept in `build/`. Some common configurations are kept there. Otherwise, read below for a more detailed description of the files.

## C++ Utilities

All the utilities are in `cpp_utils`. Dependency lists can be seen in `dependencies.yml`.

- `checker_utils.cpp`: Checker utilities, including a Reader subclass and `assertWA`, along with the codes.
- `exact_whitespace_mixin.cpp`: A mixin for Reader that implements the whitespace reading methods. It expects perfect whitespace. Used for validators and identical checkers.
- `gen_entry.cpp`: A starter template for generators. Pulls in some dependencies.
- `graphs.cpp`: Some graph utilities. Rather primitive.
- `identical_checker_entry.cpp`: A template for custom identical checkers. Also pulls dependencies.
- `identical_checker.cpp`: Implementation for a reader with `identical` behaviour.
- `integer_range.cpp`: A container-like class that has no storage cost, representing a range. Not technically restricted to integers.
- `list.cpp`: A convenience `List<T>` class that extends `vector`. Also contains miscellaneous list-related utils.
- `output.cpp`: Contains printing utilities. Try it with `print(3, 4, 'a', 8)`.
- `pairs.cpp`: Convert pairs to integers and back. Useful for generating random pairs.
- `random_arrays.cpp`: Generate some types of random arrays.
- `random.cpp`: Basic randomness.
- `random_set`: Utilities for sets of random numbers.
- `reader.cpp`: A base reader class, good for validating whitespace and implementing safe checkers.
- `set_with_bit.cpp`: Utilities for sets implemented as a BIT.
- `sol_entry.cpp`: An entry point for solutions.
- `standard_checker_entry.cpp`: An entry point for custom standard checkers.
- `standard_checker.cpp`: An implementation of a reader for standard-checker behaviour.
- `sys_deps.cpp`: System file dependencies.
- `test_generation.cpp`: Base classes for test generation.
- `utils.cpp`: Some basic aliases and utilities, used everywhere.
- `validation.cpp`: Some utilities for validation.
- `validator_entry.cpp`: An entry point for validators.

## Dependencies
- `g++`
- `python3`
  - See `requirements.txt`.

## Changelog

- `readEOF` is now called automatically by the destructor of readers, and as such should never be called explicitly.
