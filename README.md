<h1 align="center">
    DSGE, A very easy way to make 3DS GAMES
</h1>

> [!WARNING] 
> DSGE is still in developement, not many functions in DSGE is included and sometimes can take hours to create one, you may also experience bugs from DSGE and i'm working hard to fix any bugs before pushing to release!

#### What is this?
It's yet another 3DS Library made using C++, it's mission is to make DSGE a whole lot easier to make games for the 3DS System.

It's current working features are:
- Playing sounds.
- Customizable Text and Sprite.
- Lotta extra stuff.

#### Installation
Currently there's 1 way to install DSGE.

But first, please install devkitPro first, this is required for so many things (https://devkitpro.org/wiki/Getting_Started).

Upon finishing the installation for devkitPro, you can start with DSGE installation

##### 1. From GitHub
1. Click on this: https://github.com/NAEL2XD/DSGE/archive/refs/heads/main.zip
2. Wait until it's finished downloading.
3. Open the downloaded output, (eg: The downloads folder)
4. Extract and open the downloaded directory.
5. Assuming you have Python installed, in the directory location, press and type `cmd`.
6. In the CLI, type `python amalgamate.py -c c.json -s ""`, it should generate a dsge.hpp file.
7. Copy the `dsge.hpp` file.
8. Go to `examples`, `template` and `source` folder, and paste the file.
9. Go back 1 folder and double click on `run.bat` file and if everything should be working it should 1. launch output.3dsx by any of the emulators, or 2. it should be in the `build` and `3dsxbackups` folder..

> [!NOTE] 
> If you noticed the 3DSX file size being large when that hpp is placed, it's because of so many things to implement in a single file (lotta functions), this is normal.