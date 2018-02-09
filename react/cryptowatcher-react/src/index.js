import React from "react";
import { render } from "react-dom";
import Hello from "./Hello";

const styles = {
  fontFamily: "sans-serif"
};

const App = ({ currencies }) => (
  <div style={styles}>
    <h1>Crypto watcher</h1>
    <ul>
      {currencies.map(c => (
        <li>
          {c.name} - {c.symbol} : {Number(c.price_eur).toFixed(4)}
        </li>
      ))}
    </ul>
  </div>
);

const URL = "https://api.coinmarketcap.com/v1/ticker/?convert=EUR&limit=10";

const getFields = fields => obj => {
  return fields.reduce((acc, k) => {
    acc[k] = obj[k];
    return acc;
  }, {});
};

let fields = [
  "id",
  "name",
  "symbol",
  "rank",
  "price_usd",
  "price_btc",
  "percent_change_1h",
  "percent_change_24h",
  "percent_change_7d",
  "last_updated",
  "price_eur"
];
let currencyDecoder = getFields(fields);

let tap = fn => v => {
  fn(v);
  return v;
};

fetch(URL)
  .then(r => r.json())
  .then(json => json.map(currencyDecoder))
  .then(tap(console.log))
  .then(collection => {
    render(<App currencies={collection} />, document.getElementById("root"));
  });
