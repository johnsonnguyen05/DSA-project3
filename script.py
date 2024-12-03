import csv

# Read the second TSV file and create a dictionary with the first column as the key
# and the 4th column as the value.
def create_mapping(second_file):
    mapping = {}
    with open(second_file, newline='', encoding='utf-8') as f2:
        reader = csv.reader(f2, delimiter='\t')
        for row in reader:
            mapping[row[0]] = row[3]  # Store the 4th column value in the dictionary
    return mapping

# Read the first TSV file and modify it based on the mapping from the second file
def modify_first_file(first_file, mapping, output_file):
    with open(first_file, newline='', encoding='utf-8') as f1, \
         open(output_file, mode='w', newline='', encoding='utf-8') as out_f:
        reader = csv.reader(f1, delimiter='\t')
        writer = csv.writer(out_f, delimiter='\t')
        
        for row in reader:
            if row[0] in mapping:  # If the first column exists in the mapping
                row[3] = mapping[row[0]]  # Replace the 4th column with the mapping value
            writer.writerow(row)

# File paths
first_file = 'resources\datasets\movies_filtered.tsv'  # Replace with your actual file path
second_file = 'title.basics.tsv'  # Replace with your actual file path
output_file = 'modified_first_file.tsv'

# Create the mapping from the second file
mapping = create_mapping(second_file)

# Modify the first file and write to the output file
modify_first_file(first_file, mapping, output_file)

print(f"Modified file saved as {output_file}")
