inputs=$1
outputs=$2
if [ ! -e "$outputs" ]; then
  exit 1
fi
echo $outputs
for input_file in `find $inputs/*.desc`; do
  output_file=`echo ${outputs}/$(basename $input_file) | sed -e 's/.desc/.sol/g'`
  echo "< ${input_file} > ${output_file}"
  if [ -f ${output_file} ]; then
    # skip if the output file already exists
    echo "skip"
    continue
  fi
  python3 ./programs/simple-dfs.py ${input_file} > ${output_file}
  if [ $? != 0 ]; then
    # delete file if fails to execute the program
    echo "deleted"
    rm ${output_file}
  fi
done
