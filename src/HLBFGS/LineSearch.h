#ifndef LINE_SEARCH_H
#define LINE_SEARCH_H

#include "HLBFGS_BLAS.h"

//The following functions are from LBFGS,VA35,TNPACK,CG+.
//for details, see lbfgs.f and va35.f

/*
The license of LBFGS:

This software is freely available for educational or commercial purposes.
This software is released under the GNU Public License (GPL)
*/

/*
MCSRCH is modified a little for Preconditioned CG.
*/

/*
below is the description from the L-BFGS package which uses the same MCSRCH codes in Fortran
https://dl.acm.org/doi/10.1145/146847.146921
*/

//                     SUBROUTINE MCSRCH
//
//     A slight modification of the subroutine CSRCH of More' and Thuente.
//     The changes are to allow reverse communication, and do not affect
//     the performance of the routine.
//
//     THE PURPOSE OF MCSRCH IS TO FIND A STEP WHICH SATISFIES
//     A SUFFICIENT DECREASE CONDITION AND A CURVATURE CONDITION.
//
//     AT EACH STAGE THE SUBROUTINE UPDATES AN INTERVAL OF
//     UNCERTAINTY WITH ENDPOINTS STX AND STY.THE INTERVAL OF
//     UNCERTAINTY IS INITIALLY CHOSEN SO THAT IT CONTAINS A
//     MINIMIZER OF THE MODIFIED FUNCTION
//
//        F(X + STP*S) - F(X) - FTOL*STP*(GRADF(X)'S).
//
//     IF A STEP IS OBTAINED FOR WHICH THE MODIFIED FUNCTION
//     HAS A NONPOSITIVE FUNCTION VALUE AND NONNEGATIVE DERIVATIVE,
//     THEN THE INTERVAL OF UNCERTAINTY IS CHOSEN SO THAT IT
//     CONTAINS A MINIMIZER OF F(X + STP*S).
//
//     THE ALGORITHM IS DESIGNED TO FIND A STEP WHICH SATISFIES
//     THE SUFFICIENT DECREASE CONDITION
//
//           F(X + STP*S).LE.F(X) + FTOL*STP*(GRADF(X)'S),
//
//     AND THE CURVATURE CONDITION
//
//           ABS(GRADF(X + STP*S)'S)) .LE. GTOL*ABS(GRADF(X)'S).
//
//     IF FTOL IS LESS THAN GTOL AND IF, FOR EXAMPLE, THE FUNCTION
//     IS BOUNDED BELOW, THEN THERE IS ALWAYS A STEP WHICH SATISFIES
//     BOTH CONDITIONS.IF NO STEP CAN BE FOUND WHICH SATISFIES BOTH
//     CONDITIONS, THEN THE ALGORITHM USUALLY STOPS WHEN ROUNDING
//     ERRORS PREVENT FURTHER PROGRESS.IN THIS CASE STP ONLY
//     SATISFIES THE SUFFICIENT DECREASE CONDITION.
//
//     THE SUBROUTINE STATEMENT IS
//
//        SUBROUTINE MCSRCH(N, X, F, G, S, STP, FTOL, XTOL, MAXFEV, INFO, NFEV, WA)
//     WHERE
//
//       N IS A POSITIVE INTEGER INPUT VARIABLE SET TO THE NUMBER
//         OF VARIABLES.
//
//       X IS AN ARRAY OF LENGTH N.ON INPUT IT MUST CONTAIN THE
//         BASE POINT FOR THE LINE SEARCH.ON OUTPUT IT CONTAINS
//         X + STP*S.
//
//       F IS A VARIABLE.ON INPUT IT MUST CONTAIN THE VALUE OF F
//         AT X.ON OUTPUT IT CONTAINS THE VALUE OF F AT X + STP*S.
//
//       G IS AN ARRAY OF LENGTH N.ON INPUT IT MUST CONTAIN THE
//         GRADIENT OF F AT X.ON OUTPUT IT CONTAINS THE GRADIENT
//         OF F AT X + STP*S.
//
//       S IS AN INPUT ARRAY OF LENGTH N WHICH SPECIFIES THE
//         SEARCH DIRECTION.
//
//       STP IS A NONNEGATIVE VARIABLE.ON INPUT STP CONTAINS AN
//         INITIAL ESTIMATE OF A SATISFACTORY STEP.ON OUTPUT
//         STP CONTAINS THE FINAL ESTIMATE.
//
//       FTOL AND GTOL ARE NONNEGATIVE INPUT VARIABLES. (In this reverse
//		         communication implementation GTOL is defined in a COMMON
//		         statement.) TERMINATION OCCURS WHEN THE SUFFICIENT DECREASE
//		     CONDITION AND THE DIRECTIONAL DERIVATIVE CONDITION ARE
//		     SATISFIED.
//		
//		   XTOL IS A NONNEGATIVE INPUT VARIABLE.TERMINATION OCCURS
//		     WHEN THE RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY
//		     IS AT MOST XTOL.
//		
//		   STPMIN AND STPMAX ARE NONNEGATIVE INPUT VARIABLES WHICH
//		     SPECIFY LOWER AND UPPER BOUNDS FOR THE STEP. (In this reverse
//		         communication implementatin they are defined in a COMMON
//		         statement).
//		
//		   MAXFEV IS A POSITIVE INTEGER INPUT VARIABLE.TERMINATION
//		     OCCURS WHEN THE NUMBER OF CALLS TO FCN IS AT LEAST
//		     MAXFEV BY THE END OF AN ITERATION.
//		
//		   INFO IS AN INTEGER OUTPUT VARIABLE SET AS FOLLOWS :
//
//        INFO = 0  IMPROPER INPUT PARAMETERS.
//
//        INFO = -1  A RETURN IS MADE TO COMPUTE THE FUNCTION AND GRADIENT.
//
//        INFO = 1  THE SUFFICIENT DECREASE CONDITION AND THE
//                  DIRECTIONAL DERIVATIVE CONDITION HOLD.
//
//        INFO = 2  RELATIVE WIDTH OF THE INTERVAL OF UNCERTAINTY
//                  IS AT MOST XTOL.
//
//        INFO = 3  NUMBER OF CALLS TO FCN HAS REACHED MAXFEV.
//
//        INFO = 4  THE STEP IS AT THE LOWER BOUND STPMIN.
//
//        INFO = 5  THE STEP IS AT THE UPPER BOUND STPMAX.
//
//        INFO = 6  ROUNDING ERRORS PREVENT FURTHER PROGRESS.
//                  THERE MAY NOT BE A STEP WHICH SATISFIES THE
//                  SUFFICIENT DECREASE AND CURVATURE CONDITIONS.
//                  TOLERANCES MAY BE TOO SMALL.
//
//      NFEV IS AN INTEGER OUTPUT VARIABLE SET TO THE NUMBER OF
//        CALLS TO FCN.
//
//      WA IS A WORK ARRAY OF LENGTH N.
//
//    SUBPROGRAMS CALLED
//
//      MCSTEP
//
//      FORTRAN - SUPPLIED...ABS, MAX, MIN
//
//    ARGONNE NATIONAL LABORATORY.MINPACK PROJECT.JUNE 1983
//    JORGE J.MORE', DAVID J. THUENTE




/*
can also refer below java implementation description
http://jat.sourceforge.net/javadoc/jat/coreNOSA/algorithm/optimization/LBFGS/Mcsrch.html

Minimize a function along a search direction. This code is a Java translation of the function MCSRCH from lbfgs.f, 
which in turn is a slight modification of the subroutine CSRCH of More' and Thuente. The changes are to allow reverse 
communication, and do not affect the performance of the routine. This function, in turn, calls mcstep.
The Java translation was effected mostly mechanically, with some manual clean-up; in particular, array indices start at
0 instead of 1. Most of the comments from the Fortran code have been pasted in here as well.

The purpose of mcsrch is to find a step which satisfies a sufficient decrease condition and a curvature condition.

At each stage this function updates an interval of uncertainty with endpoints stx and sty. The interval of uncertainty 
is initially chosen so that it contains a minimizer of the modified function

      f(x+stp*s) - f(x) - ftol*stp*(gradf(x)'s).
 
If a step is obtained for which the modified function has a nonpositive function value and nonnegative derivative, then 
the interval of uncertainty is chosen so that it contains a minimizer of f(x+stp*s).
The algorithm is designed to find a step which satisfies the sufficient decrease condition

       f(x+stp*s) <= f(X) + ftol*stp*(gradf(x)'s),
 
and the curvature condition
       abs(gradf(x+stp*s)'s)) <= gtol*abs(gradf(x)'s).
 
If ftol is less than gtol and if, for example, the function is bounded below, then there is always a step which 
satisfies both conditions. If no step can be found which satisfies both conditions, then the algorithm usually 
stops when rounding errors prevent further progress. In this case stp only satisfies the sufficient decrease condition.
*/

//!LINE SEARCH ROUTINE
int MCSRCH(int *n, double *x, double *f, double *g, double *s, double *stp,
		   double *ftol, double *gtol, double *xtol, double *stpmin,
		   double * stpmax, int *maxfev, int *info, int *nfev, double *wa,
		   int *keep, double *rkeep, double *cg_dginit = 0);

//!   MCSTEP ROUTINE
/*!
*   COMPUTE A SAFEGUARDED STEP FOR A LINESEARCH AND TO
*   UPDATE AN INTERVAL OF UNCERTAINTY FOR  A MINIMIZER OF THE FUNCTION
*/

/*
The purpose of this function is to compute a safeguarded step for a linesearch and to update an interval of uncertainty for a minimizer of the function.
The parameter stx contains the step with the least function value. The parameter stp contains the current step. It is assumed that the derivative at stx is negative in the direction of the step. If brackt[0] is true when mcstep returns then a minimizer has been bracketed in an interval of uncertainty with endpoints stx and sty.

Variables that must be modified by mcstep are implemented as 1-element arrays.

Parameters:
stx - Step at the best step obtained so far. This variable is modified by mcstep.
fx - Function value at the best step obtained so far. This variable is modified by mcstep.
dx - Derivative at the best step obtained so far. The derivative must be negative in the direction of the step, that is, dx and stp-stx must have opposite signs. This variable is modified by mcstep.
sty - Step at the other endpoint of the interval of uncertainty. This variable is modified by mcstep.
fy - Function value at the other endpoint of the interval of uncertainty. This variable is modified by mcstep.
dy - Derivative at the other endpoint of the interval of uncertainty. This variable is modified by mcstep.
stp - Step at the current step. If brackt is set then on input stp must be between stx and sty. On output stp is set to the new step.
fp - Function value at the current step.
dp - Derivative at the current step.
brackt - Tells whether a minimizer has been bracketed. If the minimizer has not been bracketed, then on input this variable must be set false. If the minimizer has been bracketed, then on output this variable is true.
stpmin - Lower bound for the step.
stpmax - Upper bound for the step.
info - On return from mcstep, this is set as follows: If info is 1, 2, 3, or 4, then the step has been computed successfully. Otherwise info = 0, and this indicates improper input parameters.

*/

int MCSTEP(double *stx, double *fx, double *dx, double *sty, double *fy,
		   double *dy, double *stp, double *fp, double *dp, bool *brackt,
		   double *stpmin, double *stpmax, int *info);

#endif
