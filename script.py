# Input and output file paths
input_file = 'resources/datasets/title.basics.final.tsv'  # Replace with your input TSV file
output_file = 'output.tsv'  # File to save the modified TSV

# Open the input file and process it
with open(input_file, 'r', encoding='utf-8') as infile, open(output_file, 'w', encoding='utf-8') as outfile:
    for line in infile:
        columns = line.strip().split('\t')  # Split the line into columns based on tab
        # Remove the 4th (index 3) and 7th (index 6) columns, if they exist
        modified_columns = [col for i, col in enumerate(columns) if i not in {3, 6}]
        # Write the modified line to the output file
        outfile.write('\t'.join(modified_columns) + '\n')

print(f"Modified file saved as {output_file}.")