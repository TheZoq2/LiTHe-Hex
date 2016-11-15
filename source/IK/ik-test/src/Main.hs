module Main where

import Test.Hspec
import Test.QuickCheck

joint_1_max :: Float
joint_1_max = pi / 2

joint_1_min :: Float
joint_1_min = -pi / 2

joint_2_max :: Float
joint_2_max = pi / 2

joint_2_min :: Float
joint_2_min = -pi / 2

joint_3_max :: Float
joint_3_max = pi / 2

joint_3_min :: Float
joint_3_min = -pi / 2

main :: IO ()
main = hspec $
    describe "IK" $ do
        it "False is True, or is it... dun dun DUN" $
            False `shouldBe` True
        it "yolo" $
            True `shouldBe` True
