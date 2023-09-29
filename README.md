# TickTickDisplay2

## Setup

Rename `lib/config_example.h` to `lib/config.h` and fill out fields

## Convert images to header files

```
find images -type f \( -iname "*.png" -o -iname "*.jpg" -o -iname "*.jpeg" \) -exec sh -c 'python scrips/imgconvert.py -i "$0" -n "$(basename "$0" | cut -f1 -d".")" -o lib/images/"$(basename "$0" | cut -f1 -d".")".h -f' {} \;
```

## Attribution

Weather icons via https://tabler-icons.io/
