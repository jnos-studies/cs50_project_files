import csv
import sys
import collections


def main():

    # Check for command-line usage
    if len(sys.argv) < 3:
        print(f"Usage: python dna.py CSVFILE TEXTFILE (Current length: {len(sys.argv)})")
    # name my variables
    subjects = {}
    fieldnames = []
    sequence = ""
    dna_match = {}
    # Read database file into a variable
    with open(sys.argv[1], 'r') as csvFile:
        reader = csv.DictReader(csvFile)
        fieldnames += reader.fieldnames
        for row in reader:
            subjects[row["name"]] = {k: int(row[k]) for k in fieldnames[1:]}

    # Read DNA sequence file into a variable
    with open(sys.argv[2], 'r') as textFile:
        sequence += textFile.read()

    # Find longest match of each STR in DNA sequence
    for header in fieldnames[1:]:
        dna_match[header] = longest_match(sequence, header)

    # Check database for matching profiles
    for k in subjects.keys():
        s_values = list(subjects[k].values())
        dna_values = list(dna_match.values())

        if collections.Counter(s_values) == collections.Counter(dna_values):
            print(k)
            return
    print("No match")


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
