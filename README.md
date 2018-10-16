# Newton Fractal Generator

For general information on Newton fractals, read the
[Wikipedia page](https://en.wikipedia.org/wiki/Newton_fractal).
Due to the performance-heavy nature of this problem, using this generator
requires that you specify the function in C++ and compile the executable
yourself. (I attempted to have runtime-specified functions, but this resulted
in sever performance issues.)

Special thanks to
[this page](https://www.chiark.greenend.org.uk/~sgtatham/newton/)
which helped me figure out a nice way to shade the images.


## General Instructions for Running

You will need a C++ compiler to compile this code. I compile this code with
`g++ 5.4.0` on Ubuntu/Linux Mint. This project should work on any mainstream
OS, but the build process might need tweaks. All building is handled through
the Makefile`and`make help can be used to see the different options.

You may directly run the `generator.out`; configuration information (such as
resolution and iteration count) is stored and documented in `default.cfg`.
Depending on your processor, the configuration, function, and optimization level, the visualization should take anywhere from less than a second to 10+ seconds.
Factors that _negatively_ affect performance:
    - Non-polynomial functions
    - High resolution
    - Low optimization level
        - In ascending slowness `make o3`, `make o2`, `make o1`, `make`
    - Large (> 0.7) imaginary component in the step (`step_b`)

Nota bene: left clicking on the image will zoom in and right clicking will
zoom out. If you plan to do a lot of exploring, I recommend using `make o3`
because the extra compile time will result in a faster run time.

## Writing Custom Functions

Take a look at `src/functions.cpp`. I have some example functions there, but
feel free to insert whatever differentiable complex function you would like. As
you can see in the examples, you must specify both the function and its
derivative; if there is a mismatch, it will be obvious when the visualization
turns out poorly.


## Contributing

If you feel so inclined, you are welcome to fork and pull request this repo
or just open an issue/feature request.
