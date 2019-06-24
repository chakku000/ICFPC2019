const puppeteer = require('puppeteer');
const program = require('commander');
const process = require('process');

program
    .option('-d, --dir <target_dir>', 'ディレクトリを指定、prob-NNN.sol')
    .option('-n, --number <num>', '問題番号を指定')
    .option('-a, --all', 'すべてやる')
    .option('-f, --file <filename>', '回答ファイルを指定');

program.parse(process.argv);


targets = []

if (program.file) {
    if (!program.number) {
        console.error("Specify Number");
        process.exit(1);
    }
    let num = parseInt(program.number).toString().padStart(3, '0');
    targets.push([
        `${__dirname}/problems/prob-${num}.desc`,
        program.file
    ])
} else if (program.all) {
    if (!program.dir) {
        console.error("Specify Directory");
        process.exit(1);
    }
    for (let i = 1; i <= 300; ++i) {
        let num = parseInt(i).toString().padStart(3, '0');
        targets.push([
            `${__dirname}/problems/prob-${num}.desc`,
            `${program.dir}/prob-${num}.sol`
        ])
    }
}


(async () => {
    const browser = await puppeteer.launch({ headless: false });
    const page = await browser.newPage();
    await page.goto('https://icfpcontest2019.github.io/solution_checker/');

    let task = await page.$('#submit_task');
    let solution = await page.$('#submit_solution');
    let submit = await page.$('#execute_solution');
    let output = await page.$('#output');


    for (let [taskFile, solutionFile] of targets) {
        await task.uploadFile(taskFile);
        await page.waitForFunction('document.getElementById("output").innerHTML == "Done uploading task description"');

        await solution.uploadFile(solutionFile);
        await page.waitForFunction('document.getElementById("output").innerHTML == "Done uploading solution"');

        await submit.click();
        await page.waitForFunction('document.getElementById("output").innerHTML != "Pre-processing and validating the task..."');

        let result = (await page.evaluate(el => el.innerHTML, output)).replace(/\n/g, '');

        //console.log(result);
        if(result.indexOf("Success") === 0){
            let cost = result.replace(/[^0-9]/g,'');
            console.log(`OK ${taskFile}`,cost);
        }else{
            console.log(`NG ${taskFile}`,100000);
        }
        //console.log(`${taskFile}: ${result}`);
    }

    await browser.close();
})();
