file(READ "${input_file}" file_contents)
file(WRITE "${output_file}" "#if (${condition})\n${file_contents}\n#endif\n")
