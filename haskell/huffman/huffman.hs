module Huffman where

    import Heap
    import Data.Int
    import Data.Maybe
    import Data.Map.Strict

    type FreqTable = Map Char Int
    data HuffmanTree = Empty | HuffmanTree Int Char HuffmanTree HuffmanTree

    showWithIndent :: Int -> HuffmanTree -> String
    showWithIndent n Huffman.Empty = indent ++ "Empty\n"
        where indent = replicate n ' '
    showWithIndent n (HuffmanTree v c l r) = indent ++ (show v) ++ " '" ++ [c] ++ "'\n" ++ ll ++ rr
        where
            indent = replicate n ' '
            nextN  = n + 4
            ll     = Huffman.showWithIndent nextN l
            rr     = Huffman.showWithIndent nextN r

    instance Show HuffmanTree where
        show x = Huffman.showWithIndent 0 x

    freq :: HuffmanTree -> Int
    freq Huffman.Empty = 0
    freq (HuffmanTree x _ _ _) = x

    instance Eq HuffmanTree where
        x == y = (freq x) == (freq y)

    instance Ord HuffmanTree where
        compare x y = compare (freq x) (freq y)

    buildFreqTable :: String -> FreqTable
    buildFreqTable [] = empty
    buildFreqTable (x:xs) = insertWith (+) x 1 $ buildFreqTable xs

    initTree :: FreqTable -> MinHeap HuffmanTree
    initTree x = foldrWithKey' cb emptyMinHeap x
        where
            cb :: Char -> Int -> MinHeap HuffmanTree -> MinHeap HuffmanTree
            cb ch freq heap = Heap.insert (HuffmanTree freq ch Huffman.Empty Huffman.Empty) heap

    buildTree :: MinHeap HuffmanTree -> HuffmanTree
    buildTree x
        | Heap.size x == 0 = Huffman.Empty
        | otherwise        = fromJust $ getMin $ buildTree' x
            where
                buildTree' :: MinHeap HuffmanTree -> MinHeap HuffmanTree
                buildTree' x
                    | Heap.size x == 1 = x
                    | otherwise        = buildTree' $ Heap.insert node del2
                        where
                            l    = fromJust $ getMin x
                            del1 = Heap.deleteMin x
                            r    = fromJust $ getMin del1
                            del2 = Heap.deleteMin del1
                            node = HuffmanTree (freq l + freq r) '\0' l r