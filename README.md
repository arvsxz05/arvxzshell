<center><h1>ARVXZSHELL User Manual</h1></center>

## Description:
ARVXZSHELL is a terminal created for the compliance of CMSC 125 Project. It simulates how Window’s Command Prompt manages file system and manipulates files. It provides a command-line interface just and interprets commands like cmd.

## Commands:

### CD/CHDIR 

> #### Syntax: 
> > <strong><pre><code>cd [absolute/relative path]</code></pre></strong>
> > <strong><pre><code>chdir [absolute/relative path]</code></pre></strong>
> 
> #### Explanation:
> > <p>If there are no arguments detected, then it displays the current working directory, otherwise transfers current directory > if argument is valid.</p>
> 
> #### Examples:
> > <strong><pre><code>cd \\</code></pre></strong>
> Goes to the highest level, the root of the drive.
> 
> > <strong><pre><code>cd..</code></pre></strong>
> Goes back one directory.
> 
> > <strong><pre><code>cd Windows</code></pre></strong>
> If present, would take you into the Windows directory. Windows can be substituted with any other name.
> 
> > <strong><pre><code>cd \windows</code></pre></strong>
> If present, would first move back to the root of the drive and then go into the Windows directory.

### CLS

> #### Syntax:
> > <strong><pre><code>cls</code></pre></strong>
> 
> #### Explanation: 
> > <p>cls is a command that allows you to clear the complete contents of the screen and leave only a prompt.</p>
> 
> #### Examples:
> > <strong><pre><code>cls</code></pre></strong>
> Running the cls command at the command prompt would clear your screen of all previous text and only return the prompt.

### CMD	

> #### Syntax:
> > <strong><pre><code>cmd</code></pre></strong>
> 
> #### Explanation: 
> > <p>Starts a new instance of the command interpreter.</p>

### COPY	

> #### Syntax: 
> > <strong><pre><code>COPY [source] [destination]</code></pre></strong>
> 
> #### Explanation: 
> > <p>Allows you to copy one or more files to an alternate location.</p>
> 
> #### Examples:
> > <strong><pre><code>copy *.txt c:\\</code></pre></strong>
> In the above copy command we are using a wildcard to copy all .txt files (multiple files) from the current directory to the c:\ root directory.
> 
> > <strong><pre><code>copy *.txt another_dir\concatenated.txt</code></pre></strong>
> To copy the contents of all .txt files (multiple files) from the current directory to the text file named concatenated.txt found in another_dir folder.
> 
> > <strong><pre><code>copy "computer hope.txt" hope</code></pre></strong>
> Copy the file "computer hope.txt" into the hope directory. Whenever dealing with a file or directory with a space, it must be surrounded with quotes. Otherwise, you'll get the "The syntax of the command is incorrect." error.

### DATE

> #### Syntax: 
> > <strong><pre><code>DATE [mm-dd-yy | mm/dd/yy | mm-dd-yyyy | mm/dd/yyyy]</code></pre></strong>
> 
> #### Explanation: 
> > <p>The date command can be used to look at the current date of the computer as well as change the date to an alternate date. Type DATE without parameters to display the current date setting and a prompt for a new date. Press Enter to keep the same date.</p>
> 
> #### Examples: 
> > <strong><pre><code>date</code></pre></strong>
> Display the current date and prompt for a new date. If no date is entered, the current date will be kept.

### DEL

> #### Syntax: 
> > <strong><pre><code>DEL names</code></pre></strong>
> 
> #### Explanation: 
> > <p>Del is a command used to delete files from the computer.</p>
> 
> #### Examples:
> > <strong><pre><code>del test.tmp</code></pre></strong>
> Deletes the test.tmp in the current directory, if the file exists.
> 
> > <strong><pre><code>del test.tmp test2.txt windows\test.tmp</code></pre></strong>
> Deletes the test.tmp & test2.txt in the current directory and delete test.tmp in the window directory, if the file exists.
> 
> > <strong><pre><code>del c:\windows\test.tmp</code></pre></strong>
> Delete the c:\windows\test.tmp in the Windows directory if it exists.
> 
> > <strong><pre><code>del c:\windows\temp\*.*</code></pre></strong>
> The * (asterisks) is a wild character. *.* indicates that you would like to delete all files in the c:\windows\temp directory.

### DIR

> #### Syntax:
> > <strong><pre><code>DIR [drive:][path][file name]</code></pre></strong>
> 
> #### Explanation: 
> > <p>The dir command allows you to see the available files and directories in the current directory. The dir command also shows the last modification date and time, as well as the file size.</p>
> 
> #### Examples:
> > <strong><pre><code>dir</code></pre></strong>
> Lists all files and directories in the current directory. By default, the dir command lists the files and directories in alphabetic order.
> 
> > <strong><pre><code>dir *.exe</code></pre></strong>
> The above command lists any file that ends with the .exe file extension. See the wildcard definition for further wildcard examples.
> 
> > <strong><pre><code>dir *.txt *.doc</code></pre></strong>
> The above is using multiple filespecs to list any files ending with .txt and .doc in one command.
> 
> > <strong><pre><code>dir anotherdir</code></pre></strong>
> The above lists all files and directories in the another directory

### MKDIR 

> #### Syntax:
> > <strong><pre><code>MKDIR [drive:]path</code></pre></strong>
> 
> #### Explanation:
> > <p>Allows you to create directories in MS-DOS.</p>
> 
> #### Examples
> > <strong><pre><code>mkdir test</code></pre></strong>
> The above example creates the "test" directory in the current directory.
> 
> > <strong><pre><code>mkdir "computer hope"</code></pre></strong>
> The above command would create a directory called "computer hope", if you want a space in your directory name it must be surrounded in quotes.
> 
> > <strong><pre><code>mkdir c:\test</code></pre></strong>
> Create the "test" directory in the c:\ directory.
> 
> > <strong><pre><code>mkdir test1 test2</code></pre></strong>
> Create the "test1" and “test2” directory in the current directory.

### MOVE 

> #### Syntax:
> > <strong><pre><code>MOVE [drive:][path]filename destination</code></pre></strong>
> 
> #### Explanation:
> > <p>Allows you to move files or directories from one folder to another, or from one drive to another.</p>
> 
> #### Examples
> > <strong><pre><code>move "computer hope" example</code></pre></strong>
> If your directory name has a space, it must be surrounded with quotes, otherwise you will get a "The syntax of the command is incorrect." error message. In the above example, this command would move the "computer hope" directory into the example directory contained in the same directory (current directory).

### RENAME 

> #### Syntax:
> > <strong><pre><code>RENAME [drive:][path][filename1] [filename2]</code></pre></strong>
> 
> #### Explanation:
> > <p>Used to rename files and directories from the original name to a new name.</p>
> 
> #### Examples
> > <strong><pre><code>rename "computer hope.txt" example.txt</code></pre></strong>
> Rename the file "computer hope.txt" to example.txt.

### RMDIR

> #### Syntax:
> > <strong><pre><code>RMDIR [drive:]path [\q]</code></pre></strong>
> 
> #### Explanation:
> > <p>Removes an empty directory in MS-DOS. To delete directories with files or directories within them the user must use the > deltree (\q) command</p>
> 
> #### Examples
> > <strong><pre><code>rmdir c:\full</code></pre></strong>
> If a directory contains files or folders when attempting to delete the directory you will receive "The directory is not empty." error message.
> 
> > <strong><pre><code>rmdir full \q</code></pre></strong>
> This will delete all the directory “full” and all its contents

### TIME

> #### Syntax:
> > <strong><pre><code>TIME [time]</code></pre></strong>
> 
> #### Explanation:
> > <p>Allows the user to view and edit the computer's time.</p>
> 
> #### Examples
> > <strong><pre><code>time</code></pre></strong>
> View the computer’s time

### TYPE 

> #### Syntax:
> > <strong><pre><code>TYPE [drive:][path]file name</code></pre></strong>
> 
> #### Explanation:
> > <p>Allows the user to see the contents of a file.</p>
> 
> > <strong><pre><code>type c:\autoexec.txt</code></pre></strong>
> This would allow you to look at the autoexec.txt file if it exists.