# Project

## Project goals

The primary goal of the project is to obtain experience in identifying and analyzing alternate approaches to efficient solution of computational problems on a single core and on multiple nodes of a parallel computer. You may use techniques studied in class and another techniques that you learn on your own. A secondary goal is to perform a thorough performance analysis and present your results in a professional manner.

## Project tasks

You should identify some sequential application that you wish to parallelize. This may be your own code, or code written by others. Generally, it is hard to understand code written by others, and so you need to read the code before deciding to do this.

You should optimize the code on a single core and then parallelize it. You should use OpenMP and MPI. You should evaluate your performance on at least hundreds of cores.

## Suggested topic

Identify effective optimization techniques for parallel and sequential triangular matrix multiplication and analyze their impact on performance.

## Timeline

Note: All documents must have 1-inch margins on all sides and use 12 point font.

### 1. Project topic: Due Nov 15

**Deliverable**: 1-page summary of project, clearly identifying the topic.

You should submit a pdf file that includes the following information: (i) title of the project, (ii) names of students in your group (a group may consist of up to 4 students), and (iii) a summary of the proposed project.  The summary must identify the topic, any challenges that you expect in optimization and parallelization, and ideas that you wish to try in order to overcome these.

### 2. Project progress report: Due Nov 22

**Deliverable**: 1-page report on progress.

You should submit a pdf file that includes the following information: (i) title of the project, (ii) names of students in your group, and (iii) your progress in the project so far.  At this stage, you should have completed implementation, profiling, and optimization of the sequential code. You must mention some optimizations that you tried and the performance impacts. Mention unsuccessful attempts at optimization too; they too provide insight. If you are working in a group, you must identify distinct tasks that each student took ownership of.

### 3. Project report and code: Due Dec 6

**Deliverable**: A tar file that contains all code, instructions for compiling and running the code, and project report in pdf.

Your code must include separate directories for each of the following: (i) basic sequential implementation, (ii) best optimized sequential implementation, (iii) basic parallel implementation, and (iv) best optimized parallel implementation. Each directory must include a README file that explains how the code should be compiled and run. The parent directory must contain a LOG.txt file, as with the homework assignments.

Your project report would typically be around 5 pages, including figures and references. You should start with the title, names of the authors, and an abstract summarizing your accomplishments in the project.

Your main text should contain sections (i) introducing the problem and its relevance, (ii) clearly defining and explaining the problem, (iii) explaining the main sequential and parallel algorithms, including pseudo-codes for them, (iii) describing different optimizations that you tried and their impact on performance and scalability, including graphs or tables presenting your results, and (iv) a conclusion section summarizing what your learnt from the project. Your results section should include insight into possible causes for performance improvements or lack of performance improvement. Note that this report should be self-contained; you should not assume that I recall your previous reports.

## Grading

You will be graded for sequential optimizations, efficient and scalable parallelization, and for the quality of presentation in your reports, with the former two being more important. You will also be graded for clearly identifying which optimization were effective and insight on why they were effective. It is important to obtain good speedups. But you will also get credit for interesting optimization ideas that did not work out, especially if you show understanding for why they did not work out. Results that are accompanied by some understanding of the reason for them will generally get more credit that just presenting a result without any insight into them.
