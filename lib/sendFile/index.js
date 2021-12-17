const fs = require('fs')
const axios = require('axios')

const filePath = __dirname + '/public/index.html';
const url = "http://192.168.192.109";

(() => {
    const file = fs.readFile(filePath, 'utf8', async(err, data) => {
        if (err) {
            throw err
        }
        await axios.post(url, data)
    })
})()