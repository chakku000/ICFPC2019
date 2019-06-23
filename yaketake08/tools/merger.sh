base=$1
src=$2
if [ ! -e "$src" ]; then
  exit 1
fi
echo $base vs $src
for input_file in `find $base/*.sol`; do
  diff_file=${src}/$(basename $input_file)
  if [ ! -f ${diff_file} ]; then
    # skip if the output file already exists
    echo "skip"
    continue
  fi
  sz_base=`du -b ${input_file} | cut -f 1`
  sz_diff=`du -b ${diff_file} | cut -f 1`
  if [ ${sz_base} -gt ${sz_diff} ]; then
    echo "${input_file} vs ${diff_file}"
    echo "copied (${sz_base} > ${sz_diff})"

    echo "cp ${diff_file} ${input_file}"
    cp ${diff_file} ${input_file}
  fi
done
