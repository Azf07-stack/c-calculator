# Use the official GCC image so Render knows how to compile C code
FROM gcc:latest

# Set the working directory inside the cloud container
WORKDIR /app

# Copy our backend C file into the container
COPY backend/main.c .

# Compile the C code into an executable
RUN gcc main.c -o calc_server

# Tell Docker how to start our server
CMD ["./calc_server"]