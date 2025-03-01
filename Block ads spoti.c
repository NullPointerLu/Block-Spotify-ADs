#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <tlhelp32.h>

#pragma region menu
void print_menu() {
    printf("****************************************\n");
    printf("      BLOCK ADS ON SPOTIFY              \n");
    printf("****************************************\n");
    printf("1. Block ads\n");
    printf("2. Check if ads are blocked\n");
    printf("3. Exit\n");
    printf("****************************************\n");
    printf("Choose an option: ");
}
#pragma endregion

#pragma region spotify running
int is_spotify_running() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (_stricmp(entry.szExeFile, "Spotify.exe") == 0) {
                CloseHandle(snapshot);
                return 1;
            }
        }
    }

    CloseHandle(snapshot);
    return 0;
}
#pragma endregion

#pragma region kill spotify
void kill_spotify() {
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (_stricmp(entry.szExeFile, "Spotify.exe") == 0) {
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, entry.th32ProcessID);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 9);
                    CloseHandle(hProcess);
                    printf("Spotify has been closed.\n");
                    break;
                }
            }
        } while (Process32Next(snapshot, &entry) == TRUE);
    }
    CloseHandle(snapshot);
}
#pragma endregion

#pragma region restart spotify
void restart_spotify() {
    // Path to the Spotify executable
    char spotify_path[MAX_PATH];
    ExpandEnvironmentStrings("%APPDATA%\\Spotify\\Spotify.exe", spotify_path, MAX_PATH);

    // Start Spotify
    if ((int)ShellExecuteA(NULL, "open", spotify_path, NULL, NULL, SW_SHOWNORMAL) > 32) {
        printf("Spotify has been restarted.\n");
    } else {
        printf("Error restarting Spotify.\n");
    }
}
#pragma endregion

#pragma region block ads
void block_ads() {
    if (is_spotify_running()) {
        kill_spotify();
    }

    FILE *hosts_file = fopen("C:\\Windows\\System32\\drivers\\etc\\hosts", "a");
    if (hosts_file == NULL) {
        perror("Error opening hosts file");
        return;
    }

    fprintf(hosts_file, "\n# Block Spotify ads\n");
    fprintf(hosts_file, "0.0.0.0 adclick.g.doubleclick.net\n");
    fprintf(hosts_file, "0.0.0.0 adeventtracker.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 ads-fa.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 analytics.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 audio2.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 b.scorecardresearch.com\n");
    fprintf(hosts_file, "0.0.0.0 bounceexchange.com\n");
    fprintf(hosts_file, "0.0.0.0 bs.serving-sys.com\n");
    fprintf(hosts_file, "0.0.0.0 content.bitsontherun.com\n");
    fprintf(hosts_file, "0.0.0.0 core.insightexpressai.com\n");
    fprintf(hosts_file, "0.0.0.0 crashdump.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 d2gi7ultltnc2u.cloudfront.net\n");
    fprintf(hosts_file, "0.0.0.0 d3rt1990lpmkn.cloudfront.net\n");
    fprintf(hosts_file, "0.0.0.0 desktop.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 doubleclick.net\n");
    fprintf(hosts_file, "0.0.0.0 ds.serving-sys.com\n");
    fprintf(hosts_file, "0.0.0.0 googleadservices.com\n");
    fprintf(hosts_file, "0.0.0.0 googleads.g.doubleclick.net\n");
    fprintf(hosts_file, "0.0.0.0 gtssl2-ocsp.geotrust.com\n");
    fprintf(hosts_file, "0.0.0.0 js.moatads.com\n");
    fprintf(hosts_file, "0.0.0.0 log.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 media-match.com\n");
    fprintf(hosts_file, "0.0.0.0 omaze.com\n");
    fprintf(hosts_file, "0.0.0.0 open.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 pagead46.l.doubleclick.net\n");
    fprintf(hosts_file, "0.0.0.0 pagead2.googlesyndication.com\n");
    fprintf(hosts_file, "0.0.0.0 partner.googleadservices.com\n");
    fprintf(hosts_file, "0.0.0.0 pubads.g.doubleclick.net\n");
    fprintf(hosts_file, "0.0.0.0 redirector.gvt1.com\n");
    fprintf(hosts_file, "0.0.0.0 s0.2mdn.net\n");
    fprintf(hosts_file, "0.0.0.0 securepubads.g.doubleclick.net\n");
    fprintf(hosts_file, "0.0.0.0 spclient.wg.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 tpc.googlesyndication.com\n");
    fprintf(hosts_file, "0.0.0.0 v.jwpcdn.com\n");
    fprintf(hosts_file, "0.0.0.0 video-ad-stats.googlesyndication.com\n");
    fprintf(hosts_file, "0.0.0.0 weblb-wg.gslb.spotify.com\n");
    fprintf(hosts_file, "0.0.0.0 www.googleadservices.com\n");
    fprintf(hosts_file, "0.0.0.0 www.googletagservices.com\n");
    fprintf(hosts_file, "0.0.0.0 www.omaze.com\n");

    fclose(hosts_file);
    printf("Ads successfully blocked!\n");

    // Restart Spotify
    restart_spotify();
}
#pragma endregion

#pragma region check ads blocked
void check_ads_blocked() {
    FILE *hosts_file = fopen("C:\\Windows\\System32\\drivers\\etc\\hosts", "r");
    if (hosts_file == NULL) {
        perror("Error opening hosts file");
        return;
    }

    char line[256];
    int ads_blocked = 0;

    while (fgets(line, sizeof(line), hosts_file)) {
        if (strstr(line, "adclick.g.doubleclick.net") ||
        strstr(line, "adeventtracker.spotify.com") ||
        strstr(line, "ads-fa.spotify.com") ||
        strstr(line, "analytics.spotify.com") ||
        strstr(line, "audio2.spotify.com") ||
        strstr(line, "b.scorecardresearch.com") ||
        strstr(line, "bounceexchange.com") ||
        strstr(line, "bs.serving-sys.com") ||
        strstr(line, "content.bitsontherun.com") ||
        strstr(line, "core.insightexpressai.com") ||
        strstr(line, "crashdump.spotify.com") ||
        strstr(line, "d2gi7ultltnc2u.cloudfront.net") ||
        strstr(line, "d3rt1990lpmkn.cloudfront.net") ||
        strstr(line, "desktop.spotify.com") ||
        strstr(line, "doubleclick.net") ||
        strstr(line, "ds.serving-sys.com") ||
        strstr(line, "googleadservices.com") ||
        strstr(line, "googleads.g.doubleclick.net") ||
        strstr(line, "gtssl2-ocsp.geotrust.com") ||
        strstr(line, "js.moatads.com") ||
        strstr(line, "log.spotify.com") ||
        strstr(line, "media-match.com") ||
        strstr(line, "omaze.com") ||
        strstr(line, "open.spotify.com") ||
        strstr(line, "pagead46.l.doubleclick.net") ||
        strstr(line, "pagead2.googlesyndication.com") ||
        strstr(line, "partner.googleadservices.com") ||
        strstr(line, "pubads.g.doubleclick.net") ||
        strstr(line, "redirector.gvt1.com") ||
        strstr(line, "s0.2mdn.net") ||
        strstr(line, "securepubads.g.doubleclick.net") ||
        strstr(line, "spclient.wg.spotify.com") ||
        strstr(line, "tpc.googlesyndication.com") ||
        strstr(line, "v.jwpcdn.com") ||
        strstr(line, "video-ad-stats.googlesyndication.com") ||
        strstr(line, "weblb-wg.gslb.spotify.com") ||
        strstr(line, "www.googleadservices.com") ||
        strstr(line, "www.googletagservices.com") ||
        strstr(line, "www.omaze.com")) {
            ads_blocked = 1;
            break;
        }
    }

    fclose(hosts_file);

    if (ads_blocked) {
        printf("Ads are already blocked.\n");
    } else {
        printf("Ads are not blocked.\n");
    }
}
#pragma endregion

int main() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    int choice;
    while (1) {
        print_menu();
        scanf("%d", &choice);

#pragma region switch
        switch (choice) {
            case 1:
                block_ads();
                break;
            case 2:
                check_ads_blocked();
                break;
            case 3:
                printf("Exiting the program.\n");
                exit(0);
                break;
            default:
                printf("Invalid option. Please try again.\n");
        }
#pragma endregion

        // Wait for the user to press Enter to continue
        printf("Press Enter to continue...\n");
        getchar(); // Absorb the newline character left in the buffer
        getchar(); // Wait for the user to press Enter
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }
    return 0;
}
