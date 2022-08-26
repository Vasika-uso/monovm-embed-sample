#! /bin/bash

# Usage:
# gen-managed.sh out/CSharpSample/bin/Debug/net5.0/osx-x64/publish > out/native/generated/platform-assemblies.h

if [ $# -ne 1 ]; then
    echo "Usage: $0 PATH_TO_PUBLISH_DIR"
    exit 1
fi

d=$(realpath $1);

if [ "z${d}" = z -o ! -d "${d}" ] ; then
    echo publish dir ${d} does not exist
    exit 1
fi

cat - <<EOF
/*
 * DO NOT EDIT
 * This file was generated by gen-managed.sh $d
 */
#define CORELIB_FULL_PATH "C:\\Users\\user\\Documents\\GitHub\\MonoVMEmbed\\out"
EOF



for i in $d/*.dll ; do
    b=$(basename $i)
    if [ "$b" == "System.Private.CoreLib.dll" ] ; then
	continue
    fi
    echo "TRUSTED_PLATFORM_ASSEMBLY(\"$i\")"
done

exit 0

