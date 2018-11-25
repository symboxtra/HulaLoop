# Translations of HulaLoop #

The HulaLoop GUI and CLI have been translated into languages familiar to the dev team. These include Hindi, French (Standard), Spanish (Spain), and Polish.

Additional translations are welcomed and existing translations can always use improvement! Feel free to drop us an issuue or pull request if you see something that could be better translated.

## Limitations ##
Any exception messages found in the standalone ```hlaudio``` library are in English. In an effort to keep the Qt dependency out of the library, we pushed all translation to the ```hlcontrol``` module and above. Exceptions thrown by ```hlaudio``` can be caught and translated via a call to ```getTranslatedErrorMessage(int)``` in the ```hlcontrol``` library.

## Adding a Language ##
Assuming the Qt ```bin``` folder is on your path, run the following command from the project root:
```bash
lupdate src -recursive -ts res/translations/hulaloop_XX.ts
```

The *XX* should be replaced with the [ISO language code](https://www.andiamo.co.uk/resources/iso-language-codes) for the targeted language.

The file should now contain an entry for each of the English strings in the application.

Qt provides a tool called ```Linguist``` for translating the files. Again assuming the Qt ```bin``` folder is on your path, run the following command from the project root:
```bash
linguist res/translations/hulaloop_XX.ts
```

After the strings have been translated, the modified ```.ts``` file should be committed.

CMake builds will keep the ```.ts``` file up-to-date with the application's English strings from this point on. Rerunning the generation stage (use ```cmake ..``` from the build directory) forces the translations to update if they become stagnant.

If English is not your native tongue, you can choose to translate to a new language from one of the previously completed translations. When provided two files, the above ```linguist``` command will open the files side-by-side, allowing you to view both files simultaneously.

Please let us know if there are any issues. Thank you for your contribution!
