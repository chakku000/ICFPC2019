const puppeteer = require('puppeteer');
const glob = require('glob');
const fs = require('fs');

(async () => {
    let num = "300";

    const browser = await puppeteer.launch({ headless: false });
    const page = await browser.newPage();

    await page.goto('https://icfpcontest2019.github.io/solution_visualiser/');

    let uploadElement = await page.$('#submit_task');

    const out_path = __dirname + '/resultimage';
    fs.mkdirSync(out_path, { recursive: true });

    let problems = glob.sync(__dirname + '/problems/*.desc');
    for (let p of problems) {
        console.log(p);
        await uploadElement.uploadFile(p);

        // 🌚🌚🌚🌚🌚🌚
        let dataURL = await page.evaluate(function () {
            return new Promise(function (res) {
                let canvas = document.getElementById('canvas');
                let c = setInterval(function () {
                    let pixel = canvas.getContext('2d').getImageData(201, 10, 1, 1);
                    console.log(pixel.data)
                    if (pixel.data[2] < 140) {
                        clearInterval(c);
                        return res(canvas.toDataURL());
                    }
                }, 100);
            });
        });

        let buffer = new Buffer(dataURL.split(",")[1], 'base64');
        fs.writeFileSync(`${out_path}/${p.match(/prob-(\d+).desc$/)[1]}.png`, buffer);
    }

    await browser.close();
})();