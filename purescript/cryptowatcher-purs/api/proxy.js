const express = require('express')
const axios = require('axios')
const app = express()

app.use(allowCrossDomain)

app.get('/', function(req, res) {
  performRequestOnAPI({ coin: '' }, req.query)
    .then(json => res.json(json))
    .catch(e => console.log(`Problem with request: ${e.message}`))
})

app.get('/:coin', function(req, res) {
  const coin = req.params.coin
  performRequestOnAPI({ coin: coin }, req.query)
    .then(json => res.json(json))
    .catch(e => console.log(`Problem with request: ${e.message}`))
})

function allowCrossDomain(req, res, next) {
  res.header('Access-Control-Allow-Origin', '*')
  res.header('Access-Control-Allow-Methods', 'GET,PUT,POST,DELETE')
  res.header('Access-Control-Allow-Headers', 'Content-Type')
  next()
}

function performRequestOnAPI(params, query) {
  console.log('query', query)
  const [fst, ...keys] = Object.keys(query)
  const queryStr =
    Object.keys(query).length != 0
      ? keys.reduce(
          (acc, key) => `${acc}&${key}=${query[key]}`,
          `?${fst}=${query[fst]}`
        )
      : ''
  const url = params.coin
    ? `https://api.coinmarketcap.com/v1/ticker/${params.coin}${queryStr}`
    : `https://api.coinmarketcap.com/v1/ticker/${queryStr}`

  console.log(url)

  return axios
    .get(url, {
      responseType: 'json'
    })
    .then(r => {
      return params.coin.length ? r.data[0] : { coins: r.data }
    })
}

app.listen(8081)
