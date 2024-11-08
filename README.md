# Sleeping-Teaching-Assistant

Goals:
1. To solidify your understanding of Process Synchronization principles.
2. To solidify your understanding of POSIX threads.
3. To gain hands-on experience using mutex locks.
4. To gain hands-on experience using semaphores.
For this assignment:
You will use your knowledge of pthread and process/thread synchronization in order to
implement an application for a sleeping teaching assistant helping students.
You are to implement a related TA program and fill in three functions: void *TA_Activity(), void
*Student_Activity(void *threadID) and the main function as described below:
• TA_Activity: this function shall implement the TA’s activities including sleeping, helping
students, assigning chairs, etc.
A university computer science department has a teaching assistant (TA) who helps
undergraduate students with their programming assignments during regular office hours. The
TA’s office is rather small and has room for only one desk with a chair and computer. There are
three chairs in the hallway outside the office where students can sit and wait if the TA is
currently helping another student. When there are no students who need help during office hours,
the TA sits at the desk and takes a nap.
• Student_Activity: this function shall implement the students’ activities including waking up
the TA, checking chairs, waiting, leaving, etc.
If a student arrives during office hours and finds the TA sleeping, the student must awaken the
TA to ask for help. If a student arrives and finds the TA currently helping another student, the

student sits on one of the chairs in the hallway and waits. If no chairs are available, the student
will come back at a later time.
• main: initializes mutex lock and semaphores and creates one TA thread and N Student threads.
Refer to “pthreads_mutex&deadlock” in “Demo” section and the slides “Chp 5
conditionVars” on Canvas for more details and hints.
Starter code:
Please note: by default the skeleton programs will give you errors when you run them. This is
because they are accessing unallocated, unattached regions of shared memory. It’s your job to
fill in the appropriate functionality in the skeleton, de-noted by the TODO comments, in order
to make the programs work.
The skeleton codes for TA program can be found on Canvas. The files are as follows:
• TA.c: the skeleton code (see the TODO: comments in order to find out what to fill in)
Grading for this assignment (50 Points Total):
1. Design Document (5 points | 10% of this assignment)
a. Write in a document called “Design of Sleeping Teaching Assistant”.
b. Resource for Design Part:
c. http://www.tutorialspoint.com/software_engineering/software_design_basics.htm
d. http://www.tutorialspoint.com/software_engineering/software_analysis_design_to
ols.ht
2. Program compiles (2.5 points | 5%)
3. Correct use of semaphore (12.5 points | 25%)
4. Correct use of mutex (12.5 points | 25%)
5. Correct use of pthread (10 points | 20%)
6. All system calls are error-checked (2.5 points | 5%)
7. README file (5 points | 10%)
a. List your team members’ Names, Section Numbers, and Emails
b. How to run your program (platform, commands in each step and a screenshot of
one testing)
c. How did your team collaborate on your projects? Illustrate each member’s
contribution.
8. Late submissions shall be penalized 10%. No assignments shall be accepted after 24
hours.

Deliverables:
• This assignment MUST be completed using C or C++ on Linux.
• Please hand in your source code electronically (do not submit .o or executable code)
• You must make sure that the code compiles and runs correctly.
• Write a document called “Design of Sleeping Teaching Assistant”.
• Write a README file (text file, do not submit a .doc file) which contains
– Your Section#, Name and Email Address.
– The programming language you used (i.e. C or C++).
– How to execute your program.
– Anything special about your submission that we should take note of.
• Place all your files under one directory with a unique name (such as p1-[userid] for STA, e.g.
p1-whan).
• Tar the contents of this directory using the following command:
tar cvf [directory name].tar [directory name]
e.g. [directory name] E.g. tar -cvf p1-whan.tar p1-whan/
• Use Canvas to upload the tared file you created above.
• Turn in ONE copy (per group) to Canvas.
