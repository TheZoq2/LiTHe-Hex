module InverseKinematicsFFI
    ( legIK
    ) where

import Control.Applicative((<$>), (<*>))
import Foreign
import Foreign.Storable
import System.IO.Unsafe

data Leg = Leg
    { angle1 :: Float
    , angle2 :: Float
    , angle3 :: Float
    }

instance Storable Leg where
    alignment _ = 4
    sizeOf _ = 12
    peek ptr = Leg
        <$> peekByteOff ptr 0
        <*> peekByteOff ptr 4
        <*> peekByteOff ptr 8
    poke ptr (Leg a1 a2 a3) = do
        pokeByteOff ptr 0 a1
        pokeByteOff ptr 4 a2
        pokeByteOff ptr 8 a3

foreign import ccall "leg_ik"
    leg_ik :: Float -> Float -> Float -> Leg

legIK :: (Float, Float, Float) -> (Float, Float, Float)
legIK (x, y, z) = unsafePerformIO $
    alloca $ \a1Ptr ->
    alloca $ \a2Ptr ->
    alloca $ \a3Ptr -> do
        leg_ik x y z a1Ptr a2Ptr a3Ptr
        (,,) <$> peek a1Ptr <*> peek a2Ptr <*> peek a3Ptr
