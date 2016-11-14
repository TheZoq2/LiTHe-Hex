module IK exposing (calcLegIK)

import Math.Vector3 exposing (..)
import Constants exposing (..)
import Debug

calcLegIK : Vec3 -> ( Float, Float, Float )
calcLegIK pos =
    let
        clickAngle =
            atan2 -(getZ pos) (getX pos)

        horizontalAngle =
            clickAngle |> max minAngle |> min maxAngle

        kneePos =
            vec3 (thighLength * cos horizontalAngle)
                0
                -(thighLength * sin horizontalAngle)

        kneeToeDistance =
            min (distance kneePos pos) (ankleLength + footlength)

        kneeToeVec =
            Debug.log "kneeToeVec " scale kneeToeDistance (direction kneePos pos)

        alpha =
            acos ((getY kneeToeVec) / kneeToeDistance)
                + acos
                    ((footlength ^ 2 - ankleLength ^ 2 - kneeToeDistance ^ 2)
                        / (-2 * ankleLength * kneeToeDistance)
                    )
                - pi / 2

        beta =
            acos
                ((kneeToeDistance ^ 2 - footlength ^ 2 - ankleLength ^ 2)
                    / (-2 * footlength * ankleLength)
                )
                - pi
    in
        ( horizontalAngle, alpha, beta )

