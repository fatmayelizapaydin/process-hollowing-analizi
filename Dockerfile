FROM gcc:latest
COPY . /usr/src/myapp
WORKDIR /usr/src/myapp
RUN g++ -o myapp src/main.cpp || echo "Windows API oldugu icin Linux'ta derlenemedi ama mimari hazir."
CMD ["./myapp"]
