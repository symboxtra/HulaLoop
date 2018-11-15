# Translations of HulaLoop #

The HulaLoop GUI and CLI have been translated into languages familiar to the dev team. These include Hindi, French (Standard), Spanish (Spain), and Polish.

Additional translations are welcomed and existing translations can always use improvement! Feel free to drop us an issuue or pull request if you see something that could be better translated.

## Limitations ##
Any exception messages found in the standalone ```hlaudio``` library are in English. In an effort to keep the Qt dependency out of the library, we pushed all translation to the ```hlcontrol``` module and above. Exceptions thrown by ```hlaudio``` can be caught and translated via a call to ```getTranslatedErrorMessage(int)``` in the ```hlcontrol``` library.

## Adding a Language ##
Create an empty file named ```hulaloop_XX.ts``` in the ```res/translation``` directory. The *XX* should be replaced with the [ISO language code](https://www.andiamo.co.uk/resources/iso-language-codes) for the targeted language.

After the file has been added, a normal build will fill the ```.ts``` file with the English strings for translation. After the strings have been translated, the modified ```.ts``` file should be committed.