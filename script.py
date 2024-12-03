# Input and output file paths
input_file = 'resources/datasets/dataset.final.tsv'  # Replace with your actual TSV file
output_file = 'filtered_output.tsv'  # File to save the filtered rows

# Open the input file, process it, and write to the output file
with open(input_file, 'r', encoding='utf-8') as infile, open(output_file, 'w', encoding='utf-8') as outfile:
    for line in infile:
        columns = line.strip().split('\t')  # Split the line into columns based on tab
        if len(columns) >= 9:  # Ensure there are at least 9 columns
            try:
                # Check if the 9th column value is greater than or equal to 1000
                if int(columns[8]) >= 1000:
                    outfile.write(line)  # Write the line if the condition is met
            except ValueError:
                # If the 9th column isn't an integer, skip the line (or handle as needed)
                pass

print(f"Filtered file saved as {output_file}.")