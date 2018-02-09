<template>
	<div id="app">
		<h1>Crypto watcher</h1>
		<ul>
				<li v-for="c in currencies">
					{{c.name}} - {{c.symbol}} : {{Number(c.price_eur).toFixed(4)}}
				</li>
		</ul>
	</div>
</template>

<script>
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

	export default {
    name: 'app',
		data () {
    	return {
				currencies: []
			}
		},
		mounted: function () {
			this.getCurrencies()
		},
		methods: {
			getCurrencies() {
				return fetch(URL)
				.then(r => r.json())
				.then(json => json.map(currencyDecoder))
				.then(tap(console.log))
				.then(collection => {
					this.currencies = collection
				})
			}
		},
		computed: {
    	computedValue() {
				return true
			}
		}
  }
</script>

<style>
	#app {
		font-family: 'Avenir', Helvetica, Arial, sans-serif;
		-webkit-font-smoothing: antialiased;
		-moz-osx-font-smoothing: grayscale;
		color: #2c3e50;
	}
</style>