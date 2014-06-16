Created: Sat Jun 14 11:34:42 2014
By: nsmith
Using Template: Marmalade Quick Project

Example for the Quick wrapper for s3eAndroidFullscreen extension.

Requires QAndroidFullscreen to be built into the Quick binaries.
Pre-built versions of some of those are included in quick/target if
you are too lazy to re-build yourself.

If you ge a crash running after updating the targets, check that you are explicitly
setting fonts when creating labels in your code. This is a know bug when rebuilding Quick.
