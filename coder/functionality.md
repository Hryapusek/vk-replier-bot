Json:
- Password in symmetric encryption
- Path to source file
- Path to encoded file
- Path to decode pairs -> This file should be encrypted with password in symmetric encryption

States:
- File loaded
- File not loaded yet

# What program should do:
1. We should load file in format
```csv
Абраамян Александр Манвелович  2342.32
```

And generate two files. First is something like this:
```text
32723429834 2342.32
```

Second is:
```json
{
  "people": {
    "Абраамян Александр Манвелович": 32723429834
  }
}
```

Then program should show list of people that it loaded. Like second json file.
Also it should allow to search by name to obtain the number.

2. When we start the program - it should try to preload the file.
