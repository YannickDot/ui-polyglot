module Main where

import Prelude hiding (div)

import Control.Monad.Eff (Eff)
import Control.Monad.Eff.Class (liftEff)
import Data.Either (Either(..))
import Data.Foreign (ForeignError)
import Data.List (List)
import Data.List.Types (NonEmptyList)
import Network.HTTP.Affjax (get)
import Pux (CoreEffects, EffModel, start)
import Pux.DOM.Events (onClick)
import Pux.DOM.HTML (HTML)
import Pux.Renderer.React (renderToDOM)
import Simple.JSON (readJSON) as JSON
import Text.Smolder.HTML (button, div, span)
import Text.Smolder.Markup (text, (#!))

data Event e a = FetchData | ReceiveData a | GotError e

data RemoteData e a =
    NotAsked
  | Loading
  | Failure e
  | Success a

type Coin = {
  id :: String,
  name :: String,
  symbol :: String,
  price_usd :: Number,
  rank :: String
}

type State = {
  status :: RemoteData String Coin
}

parseCoin :: String -> Either (NonEmptyList ForeignError) Coin
parseCoin s = JSON.readJSON s

endPoint :: String
endPoint = "https://api.coinmarketcap.com/v1/ticker/bitcoin?convert=EUR&limit=10"

getCoin = do
  res <- get $ endPoint
  case parseCoin res.response of
    Right json -> do
      pure $ ReceiveData json
    Left e -> do
      pure $ GotError "Oh no"

-- | Return a new state (and effects) from each event
foldp :: ∀ fx. Event -> State -> EffModel State Event fx
foldp FetchData st = {
  state: st { status = Loading },
  effects: [ getCoin ]
}

foldp (ReceiveData json) st = {
  state: st { status = Loading },
  effects: []
}

foldp (GotError err) st = {
  state: st { status = Loading },
  effects: []
}


-- -- | Return markup from the state
-- view :: State -> HTML Event
-- view state =
--   div do
--     button #! onClick (const FetchData) $ text "Get coins!"
--     span $ text ("Hi")
--
-- -- | Start and render the app
-- main :: ∀ fx. Eff (CoreEffects fx) Unit
-- main = do
--   app <- start
--     { initialState: NotAsked
--     , view
--     , foldp
--     , inputs: []
--     }
--
--   renderToDOM "#app" app.markup app.input
