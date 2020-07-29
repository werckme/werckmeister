#!/usr/bin/env python3
import os.path as path
from os import system as execute

Template_Extension = ".template"
SheetTemplate = "preview.sheet"
TempSheet = "_temp.sheet"
PITCH_MAP_FILE = "defaultMIDI.pitchmap"
TARGET_MIDI_DEVICE_ID = 0
PLAYER_EXE = "sheetp"
RECORD_EXE = "record_mp3.sh"
META_VALUE_PRIORITY_INSTRUMENT = "drums"
ROOT_PATH = ""

def get_template_meta_comment_lines(file):
    with open(file, "r") as f:
        while True:
            line = f.readline()
            if not line:
                break
            if line.strip().find("--") != 0:
                break
            yield line.replace("--", "").strip()

def get_template_meta_comments(file):
    lines = get_template_meta_comment_lines(file)
    return { x[0].strip(): x[1].strip() for x in (line.split(':') for line in lines)  }

def aggregate_metas(templates):
    result = {}
    for template in templates:
        for key in template.meta.keys():
            if key not in result or template.instrument == META_VALUE_PRIORITY_INSTRUMENT:
                result[key] = template.meta[key]
    return result

class Template:
    def __init__(self, filename):
        self.path = filename
        self.name = path.basename(self.path)
        self.name = path.splitext(self.name)[0]
        self.__path_segments__ = self.name.split('.')
        if len(self.__path_segments__) < 2:
            raise RuntimeError(f"{filename} has wrong filename format")
    def __str__(self):
        return self.name
    def __repr__(self):
        return self.__str__()
    @property
    def style(self):
        return self.__path_segments__[1]
    @property
    def instrument(self):
        return self.__path_segments__[0]
    @property
    def meta(self):
        return get_template_meta_comments(self.path)

class Style:
    def __init__(self, name, templates = []): 
        self.templates = templates
        self.name = name

    def __str__(self):
        return f"{self.name} ({len(self.templates)})"
    def __repr__(self):
        return self.__str__()
    @property
    def meta(self):
        return aggregate_metas(self.templates)

class SheetGenerator:
    def __init__(self, infile=SheetTemplate): 
        self.infile = infile
        self.txt = self.input_text
        self.templates = []
        self.part = "normal"
        self.midiDeviceId = TARGET_MIDI_DEVICE_ID
        self.chords = ["Cmaj7", "Fmaj7", "G7", "Cmaj7"]
        self.meta = {}

    @property
    def tempo(self):
        if not "tempo" in self.meta:
            return 120
        return int(self.meta["tempo"])

    @property
    def signature(self):
        if not "signature" in self.meta:
            return [4, 4]
        vals = self.meta["signature"].split("/")
        return [int(x) for x in vals]

    @property
    def input_text(self):
        with open(self.infile, "r") as f:
            return f.read()
    @property
    def usings(self):
        result = [f'using "{x.path}";' for x in self.templates]
        result.append(f'using "{PITCH_MAP_FILE}";')
        return result
    @property
    def doc_config(self):
        return [f"tempo: {self.tempo};"]

    @property
    def accomp(self):
        templates = ['\t' + f'{x.name}.{self.part}' for x in self.templates]
        endl = "\n"
        return [
            f"/signature: {self.signature[0]} {self.signature[1]}/",
            f"/template: {endl.join(templates)}/",
            " | ".join(self.chords)
        ]

    def generate(self):
        self.meta = aggregate_metas(self.templates)
        res = self.txt
        res = res.replace("$DEVICE_ID", str(self.midiDeviceId))
        res = res.replace("$USINGS"           , "\n" .join(self.usings))
        res = res.replace("$DOC_CONFIG"       , "\n" .join(self.doc_config))
        res = res.replace("$ACCOMP"           , "\n" .join(self.accomp))
        return res

class Player:
    def __init__(self): 
        self.player_exe = PLAYER_EXE
        self.record_exe = RECORD_EXE

    def play(self, filename):
        execute(f"{self.player_exe} {filename}")

    def record(self, filename, outname):
        execute(f"{self.record_exe} {filename} {outname}")


def is_sheetfile(filename):
    path_ext = path.splitext(filename)
    return len(path_ext) >=2 and path_ext[1] == Template_Extension

def get_template_files(in_dir):
    import os
    arr = os.listdir(in_dir)
    arr = filter(is_sheetfile, arr)
    return [path.join(in_dir, x) for x in arr]

def get_styles_of_folder(in_dir):
    template_files = get_template_files(in_dir)
    templates = [Template(x) for x in template_files]
    styles = { x.style: list(filter(lambda t: t.style == x.style, templates)) for x in templates }
    styles = [Style(x, styles[x]) for x in styles.keys()]
    return styles

def perform(style, **args):
    import os
    global ROOT_PATH
    ROOT_PATH = os.path.dirname(os.path.abspath(__file__ ))
    generator = SheetGenerator(path.join(ROOT_PATH, SheetTemplate))
    player = Player()
    player.record_exe = path.join(ROOT_PATH, RECORD_EXE)
    generator.templates = style.templates
    # write file
    with open(TempSheet, "w") as f:
        f.write(generator.generate())
    if args["record"]:
        player.record(TempSheet, style.name+".mp3")
    else:
        player.play(TempSheet)
    # remove file
    os.remove(TempSheet)

if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser()
    parser.add_argument('in_directory', type=str, help='the input directory')
    parser.add_argument('--mididevice', type=int, help='the device id of the midi taret device')
    parser.add_argument('--style',      type=str, help='set a specific style to process')
    parser.add_argument('--playback', help='playback only')
    args = parser.parse_args()
    in_dir = args.in_directory
    PITCH_MAP_FILE = path.join(in_dir, PITCH_MAP_FILE)
    TARGET_MIDI_DEVICE_ID = args.mididevice if args.mididevice else 0
    styles = get_styles_of_folder(in_dir)
    for style in styles:
        if args.style and style.name != args.style:
            continue
        print(f"{style}:")
        perform(style, record=not args.playback)
    
