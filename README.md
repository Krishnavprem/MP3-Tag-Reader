# MP3-Tag-Reader

A command-line MP3 metadata reader and editor written in C that allows users to read and modify ID3v2 tags directly from MP3 files using low-level binary file operations.

📌 Overview
This project parses MP3 files to extract and update metadata such as Title, Artist, Album, Year, Genre, and Comments.
It is implemented without external libraries, focusing on system-level programming, binary file handling, and modular C design.

✨ **Features**

Read MP3 ID3v2 metadata

Edit individual tag fields

Command-line interface

MP3 format validation

Pure C implementation

Modular and readable codebase

🛠️ Tech Stack

Language: C

Concepts:

Binary File I/O
Command-line argument parsing

Structures & Enums

String manipulation
Modular programming

📂 Project Structure
MP3-Krishna/
├── main.c
├── read.c
├── edit.c
├── help.c
├── main.h
├── types.h
└── sample.mp3

▶️ Build & Run
Compile
gcc *.c -o mp3_tag_editor

View Metadata
./mp3_tag_editor -v sample.mp3

Edit Metadata
./mp3_tag_editor -e -t "New Title" sample.mp3
./mp3_tag_editor -e -a "Artist Name" sample.mp3

Help
./mp3_tag_editor -h
