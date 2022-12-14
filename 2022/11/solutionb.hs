{-# LANGUAGE LambdaCase      #-}
{-# LANGUAGE RecordWildCards #-}

import Control.Applicative ((<|>), empty)
import Data.Bool (bool)
import Data.List (foldl', sortBy)
import qualified Data.Map as M
import System.IO (hPrint, stderr)
import Text.Parsec (many, many1, manyTill, oneOf, parse, sepBy1)
import Text.Parsec (space, string)
import Text.Parsec.Char (char, digit, letter)
import Text.Parsec.Error (ParseError)
import Text.Parsec.String (Parser)
import Text.Read (readMaybe)

data Monkey = Monkey
  { items       :: [Int]
  , operation   :: Int -> Int
  , divisor     :: Int
  , targetTrue  :: Int
  , targetFalse :: Int
  , inspections :: Int
  }

number :: Parser Int
number = read <$> many1 digit

readFunction :: Char -> Int -> Int -> Int
readFunction '+' = (+)
readFunction '*' = (*)

parseOperation :: Parser (Int -> Int)
parseOperation = do
  string "new = old "
  op <- readFunction <$> oneOf ['+', '*'] <* space
  manyTill (digit <|> letter) space >>= \case
    "old" -> pure $ \old -> old `op` old
    rhs   -> maybe empty (\num -> pure $ \old -> old `op` num) $ readMaybe rhs

parseMonkey :: Parser (Int, Monkey)
parseMonkey = do
  num <- many space *> string "Monkey " *> number <* char ':'
  monkey <- Monkey
    <$> (many space *> string "Starting items: " *> sepBy1 number (string ", "))
    <*> (many space *> string "Operation: " *> parseOperation)
    <*> (many space *> string "Test: divisible by " *> number)
    <*> (many space *> string "If true: throw to monkey " *> number)
    <*> (many space *> string "If false: throw to monkey " *> number)
    <*> pure 0 <* many space
  pure (num, monkey)

parseMonkeys :: String -> Either ParseError (M.Map Int Monkey)
parseMonkeys input = M.fromList <$> parse (many parseMonkey) "" input

catchItem :: Int -> Monkey -> Monkey
catchItem item m = m { items = (items m) ++ [item] }

catchItems :: Int -> Monkey -> M.Map Int Monkey -> M.Map Int Monkey
catchItems norm Monkey {..} monkeys = foldl' throw monkeys items
  where adjustWorry = (`mod` norm) . operation
        target i = bool targetFalse targetTrue (i `mod` divisor == 0)
        throw acc item =
          let newItem = adjustWorry item
           in M.adjust (catchItem newItem) (target newItem) acc

throwItems :: Int -> Int -> M.Map Int Monkey -> M.Map Int Monkey
throwItems norm monkey monkeys = M.insert monkey n (catchItems norm m monkeys)
  where m = monkeys M.! monkey
        n = m { items = [], inspections = inspections m + length (items m) }

playRound :: Int -> M.Map Int Monkey -> M.Map Int Monkey
playRound norm monkeys = foldr (throwItems norm) monkeys [size - 1, size - 2 .. 0]
  where size = M.size monkeys

simulate :: Int -> Int -> M.Map Int Monkey -> M.Map Int Monkey
simulate 0 _ monkeys = monkeys
simulate n norm monkeys = simulate (n - 1) norm (playRound norm monkeys)

monkeyBusiness :: M.Map Int Monkey -> Int
monkeyBusiness =
  product . take 2 . sortBy (flip compare) . map inspections . M.elems

main :: IO ()
main = parseMonkeys <$> getContents >>= \case
  Left err -> hPrint stderr err
  Right monkeys ->
    let norm = M.foldr (\m acc -> divisor m * acc) 1 monkeys
     in print (monkeyBusiness (simulate 10000 norm monkeys))
