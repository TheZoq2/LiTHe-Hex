module InverseKinematicsFFI
    ( legIK
    ) where

import Foreign
import System.IO.Unsafe

foreign import ccall "ik.h leg_ik"
    leg_ik :: Float -> Float -> Float -> Ptr Float -> Ptr Float -> Ptr Float -> IO ()

legIK :: (Float, Float, Float) -> (Float, Float, Float)
legIK (x, y, z) = unsafePerformIO $
    alloca $ \a1Ptr ->
    alloca $ \a2Ptr ->
    alloca $ \a3Ptr -> do
        leg_ik x y z a1Ptr a2Ptr a3Ptr
        (,,) <$> peek a1Ptr <*> peek a2Ptr <*> peek a3Ptr
