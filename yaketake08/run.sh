inputs=$1
outputs=$2
if [ ! -e "$outputs" ]; then
  exit 1
fi
echo $outputs
for input_file in `find $inputs/*.desc`; do
  output_file=`echo ${outputs}/$(basename $input_file) | sed -e 's/.desc/.sol/g'`
  echo "< ${input_file} > ${output_file}"
  python3 ./programs/simple-dfs.py ${input_file} > ${output_file}
done
