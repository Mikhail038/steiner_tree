#!/bin/bash

for num in {0005..0030}; do
    file="dat/${num}_0000.json"
    
    if [[ ! -f "$file" ]]; then
        echo "NO SUCH <$file> FILE" >&2
        continue
    fi
    
    ./bin "$file" | awk -v f="$file" '{print f ": " $0}'
done > log