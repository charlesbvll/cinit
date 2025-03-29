# cinit

## Compilation

This project is using [`nob.h`](https://github.com/tsoding/nob.h) (from [tsoding](https://github.com/tsoding)) for compiling.

The first time you are using the project, you will need to run:

```bash
cc nob.c -o nob
```

Afterwards, you can just run the following in order to compile the project:

```bash
./nob
```

> [!NOTE]
> Running `./nob` will also recompile `nob.c` in case it has changed, so `cc nob.c -o nob` shouldn't be necessary afterwards, even if you modify `nob.c`.

## Run

Once compiled, you can run `cinit` using:

```bash
./cinit
```

> [!NOTE]
> Using `./nob && ./cinit` can be useful to both compile and run.
