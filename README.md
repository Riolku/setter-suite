# Setter Suite

A collection of tools for problem-setting, especially for use with [DMOJ](https://dmoj.ca), which you can also see [on github](https://github.com/DMOJ/judge-server).

## config.yml

`config.yml` is the file that this utility will check for many things. It is intended as a configuration for the problem. Keys include:

- `solutions`: an array of solutions. The first will be used as the reference solution whenever applicable.
- `checker`: The checker, either a builtin name (only `identical` is supported as of now), or a C++ file.
- `generator`: The generator, a file name.
- `generator_type`: The generator type, either `single` (the default), or `double`. See the `generate` script for an explanation of the differences.
- `case_counts`: an array of the number of cases in each suite.
- `case_points`: an array, of the same length as `case_counts`, indicating the points for each suite.
- `init_type`: the type of init.yml to generate. See `init-yml` for more details.
- `random_gen`: A generator to generate random test cases
- `validator`: A file to validate test cases. It should take the case as input, and return `0` on a valid case.
- `output_limit_length`: corresponds to the init.yml key that limits the output character count.

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

## C++ Utilities

All the utilities are in `cpp_utils`. Dependency lists can be seen in `dependencies.yml`.

- `arrays.cpp`: This file contains a custom array class that extends `vector<T>`.
- `checker_entry.cpp`: A checker template. It pulls various things for checker utilities.
- `checker_utils.cpp`: Checker utilities, including a Reader subclass and `assertWA`, along with the codes.
- `gen_entry.cpp`: Like `checker_entry`, but for a generator.
- `output.cpp`: Contains printing utilities. Try it with `print(3, 4, 'a', 8)`.
- `random_arrays.cpp`: Extends `List<T>` for randomized operations. Helpful for generation.
- `random.cpp`: Basic randomness.
- `reader.cpp`: A reader, good for validating whitespace and implementing safe checkers.
- `sol_entry.cpp`: Like `sol_entry`, but for solutions.
- `sys_deps.cpp`: System file dependencies.
- `trees.cpp`: Generating trees.
- `types.cpp`: Type aliases.

## Dependencies
- `g++`
- `python3`
  - `cached_property`
