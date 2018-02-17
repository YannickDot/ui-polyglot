module Main where

import Prelude hiding (div)

import Control.Monad.Aff (Aff, attempt)
import Control.Monad.Aff.Console as ConsoleAff
import Control.Monad.Eff (Eff)
import Control.Monad.Eff.Console (CONSOLE)
import Control.Monad.Eff.Random (RANDOM)
import DOM (DOM)
import Data.Array (uncons)
import Data.Either (Either(..))
import Data.Foldable (for_)
import Data.Foreign (ForeignError)
import Data.List.Types (NonEmptyList)
import Data.Maybe (Maybe(..))
import Data.Tuple (Tuple(..), fst, snd)
import Network.HTTP.Affjax (AJAX, get)
import Pux (CoreEffects, EffModel, noEffects, start)
import Pux.DOM.Events (onClick)
import Pux.DOM.HTML (HTML)
import Pux.Renderer.React (renderToDOM)
import Simple.JSON (readJSON) as JSON
import Text.Smolder.HTML (button, div, h2, li, p, ul)
import Text.Smolder.HTML.Attributes (className)
import Text.Smolder.Markup (text, (!), (#!))

data RemoteData e a =
    NotAsked
  | Loading
  | Failure e
  | Success a

data Event e a =
    Reset
  | FetchData
  | FetchError e
  | ParseError e
  | GotJson a

type Coin = {
  id :: String,
  name :: String,
  symbol :: String,
  price_usd :: String,
  price_eur :: String,
  rank :: String,
  percent_change_1h :: String,
  percent_change_24h :: String,
  percent_change_7d :: String
}

type Coins = {
  coins :: Array Coin
}

type AppEffects = (console :: CONSOLE, dom :: DOM, ajax :: AJAX, random :: RANDOM)
type State = RemoteData String (Coins)
type Events = Event String (Coins)

endPoint :: String
-- endPoint = "https://api.coinmarketcap.com/v1/ticker/bitcoin?convert=EUR&limit=10"
endPoint = "http://localhost:8081/?convert=EUR&limit=30"

-- JSON Parsing Magic!
parseCoin :: String -> Either (NonEmptyList ForeignError) (Coins)
parseCoin = JSON.readJSON

getCoin :: forall a. Aff ( ajax :: AJAX, random :: RANDOM | a ) (Maybe Events)
getCoin = do
  res <- attempt $ get $ endPoint
  case res of
    Left e -> do
      pure $ Just $ FetchError $ show e
    Right {response} ->
      case parseCoin response of
        Right json -> do
          pure $ Just $ GotJson json
        Left e -> do
          pure $ Just $ ParseError $ show e

-- | Return a new state (and effects) from each event
foldp :: Events -> State -> EffModel State Events AppEffects
foldp Reset _ = noEffects $ NotAsked
foldp FetchData _ = { state: Loading, effects: [ getCoin ] }
foldp (FetchError err) _ = noEffects $ Failure err
foldp (ParseError err) _ = noEffects $ Failure err
foldp (GotJson json) _ = {
  state: Success json,
  effects: [ do
    ConsoleAff.log $ printList json.coins
    pure Nothing
  ]
}

printList :: Array Coin -> String
printList a = go a "["
  where
  go arr s =
    case uncons arr of
    Nothing -> s <> "]"
    Just {head: c, tail: []} -> go [] $ s <> c.symbol
    Just {head: c, tail} -> go tail $ s <> c.symbol <> ", "

coinItem :: Coin -> HTML Events
coinItem c =
  li do
    div ! className "flex justify-between f6" $ do
      p $ text $ c.name <> " (" <> c.symbol <> ")"
      p $ text $ "$" <> c.price_usd
      p $ text $ c.price_eur <> "€"
      p $ text $ c.percent_change_1h <> "%"


-- | Return markup from the state
view :: State -> HTML Events
view state =
  div ! className "avenir ph4 flex flex-column" $ do
    h2 ! className "f1 tc" $ text "Cryptowatcher in Purescript"
    div ! className "flex" $ do
      button ! className "bg-lightest-blue ba bw1 b--lightest-blue br2 pa3 mr3" #! onClick (const FetchData) $ text "Get coin data!"
      button ! className "bg-lightest-blue ba bw1 b--lightest-blue br2 pa3 mr4" #! onClick (const Reset) $ text "Reset"
      p $ text (snd dataToDisplay)
    div do
      ul ! className "list pl0" $ for_ (fst dataToDisplay) coinItem
  where
    dataToDisplay =
      case state of
        NotAsked          -> Tuple [] "Not asked"
        Loading           -> Tuple [] "Loading"
        Failure e         -> Tuple [] $ "Failure : " <> e
        Success { coins } -> Tuple coins "Success"

main :: Eff (CoreEffects AppEffects) Unit
main = do
  app <- start
    { initialState: NotAsked
    , view
    , foldp
    , inputs: []
    }
  renderToDOM "#pux" app.markup app.input
