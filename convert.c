// Copyright MIT License (c) 2018
// Marcel Bobolz <ergotamin.source@gmail.com>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <MagickWand/MagickWand.h>

MagickWand *new_magickwand(void);
void free_magickwand(MagickWand *wand);
void convert_png_to_raw(const char *filename, int width, int height);
void convert_raw_to_png(const char *filename, int width, int height);

MagickWand *new_magickwand(void)
{
    MagickWand *new_wand = NULL;

    MagickWandGenesis();
    (IsMagickWandInstantiated() == MagickTrue) ?
    (new_wand = NewMagickWand()) :
    (new_wand = (MagickWand *)NULL);
    if (new_wand != (MagickWand *)NULL) {
        return new_wand;
    } else {
        printf("new_magickwand() failed.");
        abort();
    }
    return NULL;
}

void free_magickwand(MagickWand *wand)
{
    ClearMagickWand(wand);
    DestroyMagickWand(wand);
    MagickWandTerminus();
    wand = (void *)0;
    return;
}

void convert_png_to_raw(const char *filename, int width, int height)
{
    MagickWand *this_wand = new_magickwand();
    char *rgbafile = strdup(filename);

    strcat(rgbafile, ".rgba\0y");

    MagickSetSize(this_wand, width, height);
    MagickSetFormat(this_wand, "RGBA");
    MagickSetDepth(this_wand, 8);
    MagickReadImage(this_wand, filename);
    MagickSetFormat(this_wand, "PNG");
    MagickWriteImage(this_wand, "again.rgba");
    free_magickwand(this_wand);
}

void convert_raw_to_png(const char *filename, int width, int height)
{
    MagickWand *this_wand = new_magickwand();

    MagickSetSize(this_wand, width, height);
    MagickSetFormat(this_wand, "RGBA");
    MagickSetDepth(this_wand, 8);
    MagickReadImage(this_wand, filename);
    MagickSetFormat(this_wand, "PNG");
    MagickWriteImage(this_wand, "newpng.png");
    free_magickwand(this_wand);
}

int main(int argc, char **argv)
{
    argc--;
    argv++;
    convert_raw_to_png(argv[0], atoi(argv[1]), atoi(argv[2]));
    return EXIT_SUCCESS;
}
