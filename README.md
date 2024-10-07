# Steganography-Encoding-Decoding-Project
# Introduction
This project provides a simple steganography tool to encode and decode text messages into and from an image file using BMP format. 
It allows hiding a secret message (text) inside an image and retrieving it later using encoding and decoding techniques.

# Features
Encoding: Embed a secret message from a text file into a BMP image.
Decoding: Extract the hidden message from a BMP image and save it to a text file.

# Prerequisites
C Compiler: You need a C compiler such as gcc installed to compile and run the project.
BMP Image: The input image must be in BMP format (uncompressed).
Text File: Text file containing the secret message to encode.

# How it Works
Encoding
The program reads the BMP image and a secret text file, and encodes the text within the image by modifying the least significant bits (LSBs) of pixel values.

Decoding
The program extracts the hidden data from an encoded BMP image by reading the LSBs of the image's pixel values.

# Files
main.c: The main driver code that processes the command-line arguments and invokes either encoding or decoding.
encode.h: Header file containing the function declarations for encoding.
decode.h: Header file containing the function declarations for decoding.
types.h: Contains the enum definitions for operation types and success/failure codes.
encode.c: The implementation of encoding functions.
decode.c: The implementation of decoding functions.

# Encoding
./a.out -e <input_image.bmp> <secret.txt> <output_image.bmp>
Example:
./a.out -e beautiful.bmp secret.txt stegno.bmp


# Decoding
./a.out -d <encoded_image.bmp> <output_text.txt>
Example:
./a.out -d stegno.bmp output.txt

# Error Handling
Displays an error message for invalid input:
INVALID OPTION
For Encode: ./a.out -e beautiful.bmp secret.txt stegno.bmp
For Decode: ./a.out -d stegno.bmp data.txt
