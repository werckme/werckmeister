## Commands
* [staccato](#staccato)

## `staccato`
---
Performs every note staccato. It is also possible to tag single notes.

 If a tag is given, only the tagged notes are performed staccato.

 ```

 /mod: staccato _forTag="stac"/
 c d e "stac"@f  only f will be performed staccato
 ```
### parameters
| name | position | description | range |
|:--- |:--- |:--- |:--- |
| forTag | - | Specifies a tag name | [a-zA-Z0-9.]+ |
| amount | - | The ammount of the staccato | 0..100 |

<br><br><br>

