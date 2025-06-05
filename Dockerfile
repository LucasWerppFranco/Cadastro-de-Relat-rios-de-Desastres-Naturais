# Use an official lightweight GCC image as build environment
FROM gcc:latest

# Set working directory inside the container
WORKDIR /app

# Copy your source code into the container
COPY main.c .

# Compile the program with linking math library
RUN gcc main.c -o main -lm

# Command to run the compiled program
CMD ["./main"]
