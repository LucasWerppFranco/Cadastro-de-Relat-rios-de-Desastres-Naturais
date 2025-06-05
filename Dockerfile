FROM gcc:latest

WORKDIR /app

COPY main.c .

RUN gcc main.c -o main -lm

CMD ["./main"]
