#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SONGS 100
#define QUEUE_SIZE 10

typedef struct {
    char name[50];
    char artist[50];
    char album[50];
} Song;


typedef struct Node {
    Song song;
    struct Node* next;
} Node;


typedef struct {
    Song queue[QUEUE_SIZE];
    int front;
    int rear;
    int count;
} Queue;

Song songs[MAX_SONGS];
int songCount = 0;
Queue playbackQueue;
void addSong();
void viewSongs();
void searchSong();
void addSongToPlaylist(Node** head);
void viewPlaylist(Node* head);
void enqueue(Queue* q, Song song);
Song dequeue(Queue* q);
void initializeQueue(Queue* q);
void playNextSong();
void menu();
void addSong() {
    if (songCount >= MAX_SONGS) {
        printf("Song list is full!\n");
        return;
    }
    printf("Enter song name: ");
    scanf(" %[^\n]", songs[songCount].name);
    printf("Enter artist name: ");
    scanf(" %[^\n]", songs[songCount].artist);
    printf("Enter album name: ");
    scanf(" %[^\n]", songs[songCount].album);
    songCount++;
    printf("Song added successfully!\n");
}
void viewSongs() {
    printf("List of all songs:\n");
    for (int i = 0; i < songCount; i++) {
        printf("%d. %s - %s [%s]\n", i + 1, songs[i].name, songs[i].artist, songs[i].album);
    }
}
void searchSong() {
    char searchTerm[50];
    printf("Enter song name, artist, or album to search: ");
    scanf(" %[^\n]", searchTerm);

    printf("Search results:\n");
    for (int i = 0; i < songCount; i++) {
        if (strstr(songs[i].name, searchTerm) || strstr(songs[i].artist, searchTerm) || strstr(songs[i].album, searchTerm)) {
            printf("%d. %s - %s [%s]\n", i + 1, songs[i].name, songs[i].artist, songs[i].album);
        }
    }
}
void deleteSongFromLibrary() {
    int songIndex;
    printf("Enter song index to delete from library: ");
    scanf("%d", &songIndex);

    if (songIndex < 1 || songIndex > songCount) {
        printf("Invalid song index!\n");
        return;
    }

    for (int i = songIndex - 1; i < songCount - 1; i++) {
        songs[i] = songs[i + 1];
    }
    songCount--;
    printf("Song deleted from library!\n");
}


void addSongToPlaylist(Node** head) {
    int songIndex;
    printf("Enter song index to add to playlist: ");
    scanf("%d", &songIndex);

    if (songIndex < 1 || songIndex > songCount) {
        printf("Invalid song index!\n");
        return;
    }

    Node* newSong = (Node*)malloc(sizeof(Node));
    newSong->song = songs[songIndex - 1];
    newSong->next = *head;
    *head = newSong;

    printf("Song added to playlist!\n");
}

void deleteSongFromPlaylist(Node** head) {
    char songName[50];
    printf("Enter the name of the song to delete from playlist: ");
    scanf(" %[^\n]", songName);

    Node *temp = *head, *prev = NULL;

    if (temp != NULL && strcmp(temp->song.name, songName) == 0) {
        *head = temp->next;
        free(temp); 
        printf("Song deleted from playlist!\n");
        return;
    }

    while (temp != NULL && strcmp(temp->song.name, songName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Song not found in playlist!\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    printf("Song deleted from playlist!\n");
}
void viewPlaylist(Node* head) {
    printf("Playlist songs:\n");
    Node* temp = head;
    int count = 1;
    while (temp != NULL) {
        printf("%d. %s - %s [%s]\n", count++, temp->song.name, temp->song.artist, temp->song.album);
        temp = temp->next;
    }
}
void initializeQueue(Queue* q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}
void enqueue(Queue* q, Song song) {
    if (q->count == QUEUE_SIZE) {
        printf("Playback queue is full!\n");
        return;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    q->queue[q->rear] = song;
    q->count++;
    printf("Song added to playback queue!\n");
}
Song dequeue(Queue* q) {
    if (q->count == 0) {
        printf("Playback queue is empty!\n");
        Song empty = {"", "", ""};
        return empty;
    }
    Song song = q->queue[q->front];
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return song;
}
void playNextSong() {
    Song song = dequeue(&playbackQueue);
    if (strlen(song.name) > 0) {
        printf("Now playing: %s - %s [%s]\n", song.name, song.artist, song.album);
    }
}
void menu() {
    int choice;
    Node* playlist = NULL;
    initializeQueue(&playbackQueue);

    while (1) {
        printf("\n--- Music Playlist Manager ---\n");
        printf("1. Add Song to Library\n");
        printf("2. View All Songs\n");
        printf("3. Search Song\n");
        printf("4. Delete Song from Library\n");
        printf("5. Add Song to Playlist\n");
        printf("6. Delete Song from Playlist\n");
        printf("7. View Playlist\n");
        printf("8. Add Song to Playback Queue\n");
        printf("9. Play Next Song\n");
        printf("10. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addSong();
                break;
            case 2:
                viewSongs();
                break;
            case 3:
                searchSong();
                break;
            case 4:
                deleteSongFromLibrary();
                break;
            case 5:
                addSongToPlaylist(&playlist);
                break;
            case 6:
                deleteSongFromPlaylist(&playlist);
                break;
            case 7:
                viewPlaylist(playlist);
                break;
            case 8: {
                int songIndex;
                printf("Enter song index to add to playback queue: ");
                scanf("%d", &songIndex);

                if (songIndex < 1 || songIndex > songCount) {
                    printf("Invalid song index!\n");
                } else {
                    enqueue(&playbackQueue, songs[songIndex - 1]);
                }
                break;
            }
            case 9:
                playNextSong();
                break;
            case 10:
                printf("Exiting Music Playlist Manager. Goodbye!\n");
                return;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
}
int main() {
    menu();
    return 0;
}
