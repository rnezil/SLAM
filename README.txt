SLAM stands for Scripting Language for Academic Mathematics
and was developed by Ryland Nezil in 2023. SLAM provides the
functionality of a standard pocket calculator through either
both a command line interface or an interactive mode,
depending on the options specified on launch.

Launch SLAM:	$INSTALL_DIR/bin/SLAM [options] "expression"

SLAM provides the following launch options:

	-i	Interactive mode

	-d	Debug mode

	-g	Use degrees instead of radians

	-p=X	Set precision to X (X between 0 and 15
		inclusive)

SLAM expressions must be entered using synactically correct
SLAM script:

	-	Expressions must be mathematically sound

	-	No implicit multiplication, that is, all
		multiplication explicitly denoted with a "*"
		symbol

	-	Negation must be performed using the negation
		operator, i.e. "-5" is not valid

	-	An equals sign "=" must be used to indicate the
		end of an expression; except in non-interactive
		mode, where the equals sign will automatically
		be appended to the input string if omitted by
		the user

	-	SLAM ignores all whitespaces and newlines

Entering mathematically or syntactically incorrect expressions
will result in an informative error message being printed to
stderr. When running in non-interactive mode, such an error will
terminate the program; when running in regular mode, errors like
this will not terminate the program.

When launched in non-interactive mode, SLAM requires
an expression to be provided in quotations, by the user, at the
end of the launch command (quotations are not always necessary,
but their omittance is at the users own risk). The expression is
then evaluated and output to stdout without any extra formatting;
no newlines and no spaces, just the number, and nothing else (user
beware, this is not true when launched in debug mode). This "raw"
output format is designed to be friendly for use with external
programs and scripts. For example, the following Bash script sets
the variable x to the value 4:

	x="(SLAM_install_path/bin/SLAM "2+2")"

where SLAM_install_path represents the directory where SLAM has
been installed to. Providing an invalid expression to SLAM in
non-interactive mode will result in an error message being
outputted to stderr, and nothing be outputted to stdout.

When launched in interactive mode, SLAM displays a basic title
splash and prompts the user to enter an expression. Users must
indicate the end of each expression with an equals sign "=".
Furthermore, SLAM begins interpretation of the next expression
immediately after encountering the end of the last one. The
interactive SLAM shell can be exited by entering "Q=". Entering
and invalid expression in interactive mode will result in an error
message being outputted to stderr.

Debug mode is a sort of "add-on" feature that can be used with both
interactive and non-interactive mode. When launched in debug mode,
SLAM will print out a sequence of tokens corresponding to those
generated from the most recent input string. This sequence has the
following format:

	{ [token(1)] [token(2)] ... [token(n-1)] [token(n)] }

Numbers, parentheses and binary operators are represented directly
by these tokens, for example as [5], [(] or [+]. Unary operators
are represented simple as [F] regardless of which operator the token
corresponds to.

The precision option allows for specifying the maximum number of
digits after a decimal point that should be outputted for any given
number. The range of allowable precisions is integers [0,15]. To
launch slam with a precision of 6, the user should enter the
following:

	SLAM_install_path/bin/SLAM -p=6

where SLAM_install_path represents the directory where SLAM has been
installed to. Using the precision option has the secondary effect
of turning off scientific notation output formatting, which is the
default output format when no precision is specified.

The option to use degrees instead of radians is buggy and exists
primarily for the sake of existing. Due to the nature of floating
point numbers, most of the results produced in this mode are often
skewed and the mode overall is considered generally unreliable.

Any combination of the debug, precision, and degree options can be
used simultaneously with both interactive mode and non-interactive
mode. However, non-interactive mode requires that all options be
specified before specifying the input expression (interactive mode
does not allow specifying input expressions from the command line;
and, if an input expression is given, the expression and all
proceeding options will be ignored). Some examples of valid option
usage are shown below:

	Valid:		SLAM_install_path/bin/SLAM -d -i -p=2
(some options with interactive mode)

	Valid:		SLAM_install_path/bin/SLAM -p=2 -g -i
(options specified in different order)

	Valid:		SLAM_install_path/bin/SLAM -d -p=0 "24*cos(pi)"
(some options with non-interactive mode)

	Invalid:	SLAM_install_path/bin/SLAM "50/neg(4)" -p=0
(options specified after expression)

	Invalid:	SLAM_install_path/bin/SLAM -i "sqrt(9)" -d -g
(command line expression given in interactive mode + options specified after)

	Invalid: 	SLAM_install_path/bin/SLAM -g -p=5
(no expression given in non-interactive mode command)

where SLAM_install_path represents the directly where SLAM has been
installed.

SLAM supports the following binary operators:
	
	+	Addition operator

	-	Subtraction operator

	*	Multiplication operator

	/	Division operator

	^	Exponentiation operator

	E	"To-the-power-of-ten" operator

Most of these will be familiar to all users, but it is worth mentioning
that the last operator, the to-the-power-of-ten operator, functions
as follows:

	2E3 evaluates to (2*10^3)

	76Eneg(2) evaluates to (76*(1/10)^2)

where neg is the unary negation operator (see below).

SLAM supports the following unary operators:

	sin	Sine operator

	cos	Cosine operator

	tan	Tangent operator

	arcsin	Inverse sine operator
	
	arccos	Inverse cosine operator
	
	arctan	Inverse tangent operator

	log	Base 10 logarithm operator

	lob	Base 2 logarithm operator

	ln	Natural logarithm operator

	exp	Natural exponentiation operator

	sqrt	Square root operator

	cbrt	Cube root operator

	fac	Factorial operator

	abs	Absolute value operator

	neg	Negation operator

It is imperative that users enclose the argument of any unary
operator inside brackets, for example sqrt(3+3+3). Failure to
do so will result in a SLAM user error.

