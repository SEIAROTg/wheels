data Node a = Empty | Node a (Node a) (Node a) deriving Show
data MinHeap a = MinHeap Int (Node a) deriving Show

emptyMinHeap :: MinHeap a
emptyMinHeap = MinHeap 0 Empty

insert :: Ord a => a -> MinHeap a -> MinHeap a
insert x (MinHeap n node) = MinHeap (n+1) (_insert (n+1) x node)
    where
        _insert :: Ord a => Int -> a -> Node a -> Node a
        _insert _ x Empty        = Node x Empty Empty
        _insert n x (Node v l r) = Node vv ll rr
            where
                vv       = min x v
                downTree = mod n 2
                recur    = _insert (div n 2) (max x v)
                ll       = if downTree == 0 then recur l else l
                rr       = if downTree == 1 then recur r else r

value :: Ord a => Node a -> a
value (Node x _ _) = x

getMin :: Ord a => MinHeap a -> Maybe a
getMin (MinHeap _ Empty) = Nothing
getMin (MinHeap _ node)  = Just $ value node

deleteMin :: Ord a => MinHeap a -> MinHeap a
deleteMin (MinHeap _ (Node _ Empty Empty)) = emptyMinHeap
deleteMin (MinHeap n node) = MinHeap (n-1) (uncurry _update $ _delete n node)
    where
        _update :: Ord a => a -> Node a -> Node a
        _update v (Node _ Empty Empty) = Node v Empty Empty
        _update v (Node _ l Empty)
            | v > vl    = Node vl (_update v l) Empty
            | otherwise = Node v l Empty
            where
                vl = value l
        _update v (Node _ l r)
            | v <= vl && v <= vr = Node v l r
            | vl < vr            = Node vl (_update v l) r
            | otherwise          = Node vr l (_update v r)
            where
                vl = value l
                vr = value r

        _delete :: Ord a => Int -> Node a -> (a, Node a)
        _delete n (Node v l r)
            | n == 2       = (value l, Node v Empty Empty)
            | n == 3       = (value r, Node v l Empty)
            | otherwise    = (fst down, Node v ll rr)
                where
                    downTree = mod n 2
                    down     = _delete (div n 2) (if downTree == 0 then l else r)
                    ll       = if downTree == 0 then snd down else l
                    rr       = if downTree == 1 then snd down else r
