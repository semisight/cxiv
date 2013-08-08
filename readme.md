# CXIV

CXIV is a scheme-like language. I am building it primarily for educational
purposes. Later it may become a test bed for ideas that I have about PL design
(if I get that far).

## 1 Language Specification

*Warning: this language is, for the forseeable future, in a permanent alpha
release state. I make no guarantees as to the accuracy of this section,
although I will try to keep it up to date.*

### 1.1 Atoms

There are a few kinds of atoms (non-list primitive types) accepted by CXIV.
Atoms evaluate to themselves.

#### 1.1.1 Numbers

Ex: `-1`, `123`, `42`...

These are represented internally as signed 64-bit doubles. __There is no
integer type__--this is similar to javascript and lua. I suppose one could
recompile it with 32/64-bit integers, but it is not officially supported.

#### 1.1.2 Booleans

Ex: `#t`, `#f`.

These are the primitive true and false values. They are returned by comparison
operations.

#### 1.1.3 Characters

Ex: `#\a`, `#\Z`, `#\newline`

Any representable ASCII character may be preceded by a hash and forward slash
(`#\`) and it will be interpreted as a character. Non-representable characters
(like newline, space, etc.) may be represented by their name preceded by the
same prefix.

Only space and newline are supported at this time.

#### 1.1.4 Strings

Ex: `"hello world"`

Any double quote not preceded by the character prefix `(#\)` will be interpreted
as the beginning of a string, up until the next double quote.
