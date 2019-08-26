
from gimpfu import gimp
from gimpfu import pdb

from gimpfu import main
from gimpfu import register

from gimpfu import INDEXED
from gimpfu import PIXELS
from gimpfu import PF_FONT
from gimpfu import PF_SPINNER
from gimpfu import PF_STRING

from os.path import join

def create_base_alphabet(font, font_size, directory_base):
    suffix = '.xbm'
    for c_gen, c_prefix in ( (map(chr, xrange(ord('A'), ord('Z')+1)), 'upper'),
                             (map(chr, xrange(ord('a'), ord('z')+1)), 'lower'),
                             (map(chr, xrange(ord('0'), ord('9')+1)), 'number'), ):
        for c in c_gen:
            prefix = '%s%c' % (c_prefix, c)
            filename = '%s%s' % (prefix, suffix)

            image = gimp.Image(1, 1, INDEXED)
            image.disable_undo()

            # necessary ?
            pdb.gimp_context_push()

            gimp.set_foreground( (0.0, 0.0, 0.0) )

            x = y = border = 0
            layer = pdb.gimp_text_fontname(image, None, x, y, c, border, True, font_size, PIXELS, font)

            image.resize(layer.width, layer.height, 0, 0)

            filepath = join(directory_base, filename)
            pdb.gimp_file_save(image, layer, filepath, '?')
            #pdb.file_xbm_save(RUN_NONINTERACTIVE, image, None, filename, filepath, '', 0, 0, 0)
            # find out how to properly call file_xbm_save so we can avoid this crap
            with open(filepath, 'r+b') as file_h:
                lines = file_h.readlines()
                file_h.seek(0)
                for line in lines:
                    if '_' in line:
                        if '_hot' not in line:
                            file_h.write(line.replace('_', prefix+'_'))
                    else:
                        file_h.write(line)
                file_h.truncate()

            pdb.gimp_image_delete(image)

            # necessary ?
            pdb.gimp_context_pop()

import string
def create_base_alphabet(font, font_size, directory_base):
    suffix = '.xbm'
    for c in string.printable:
        # Only need the first guard to prevent \x0b and \x0c, but meh
        if not ' ' <= c <= '\x7f':
            continue
        prefix = 'ascii0x%02x' % ord(c)
        filename = '%s%s' % (prefix, suffix)

        image = gimp.Image(1, 1, INDEXED)
        image.disable_undo()

        # necessary ?
        pdb.gimp_context_push()

        gimp.set_foreground( (0.0, 0.0, 0.0) )

        x = y = border = 0
        layer = pdb.gimp_text_fontname(image, None, x, y, c, border, True, font_size, PIXELS, font)

        if layer is None:
            print 'Failed to handle %r' % c

        else:
            image.resize(layer.width, layer.height, 0, 0)

            filepath = join(directory_base, filename)
            pdb.gimp_file_save(image, layer, filepath, '?')
            #pdb.file_xbm_save(RUN_NONINTERACTIVE, image, None, filename, filepath, '', 0, 0, 0)
            # find out how to properly call file_xbm_save so we can avoid this section
            with open(filepath, 'r+b') as file_h:
                lines = file_h.readlines()
                file_h.seek(0)
                for line in lines:
                    if '_' in line:
                        if '_hot' not in line:
                            file_h.write(line.replace('_', prefix+'_'))
                    else:
                        file_h.write(line)
                file_h.truncate()

        pdb.gimp_image_delete(image)

        # necessary ?
        pdb.gimp_context_pop()

register(
    "create_base_alphabet",
    "Create the base alphabet XBMs for a given font/size",
    "Create the base alphabet XBMs for a given font/size",
    "WanderingGlitch",
    "WanderingGlitch",
    "2016",
    "Create base alphabet",
    "",
    [
        (PF_FONT, "font", "Font face", "Ariel"),
        (PF_SPINNER, "font_size", "Font Size", 18, (10, 100, 1)),
        (PF_STRING, "string", "Directory base", ""),
    ],
    [],
    create_base_alphabet, 
)

main()

