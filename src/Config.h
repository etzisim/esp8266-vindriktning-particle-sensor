#pragma once

#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h> 

namespace Config {
    char mqtt_server[80] = "example.tld";

    char username[24] = "";
    char password[24] = "";

    void save() {
        printf("Format FS an save new Values!\n");
        LittleFS.format();
        printf("Format FS done!\n");
        DynamicJsonDocument json(512);
        json["mqtt_server"] = mqtt_server;
        json["username"] = username;
        json["password"] = password;

        File configFile = LittleFS.open("/config.json", "w");
        if (!configFile) {
            printf("couldnt open config.json!\n");
            return;
        }

        serializeJson(json, configFile);
        configFile.close();
        printf("config.json written!\n ");
    }

    void load() {
        if (LittleFS.begin()) {

            if (LittleFS.exists("/config.json")) {
                File configFile = LittleFS.open("/config.json", "r");

                if (configFile) {
                    const size_t size = configFile.size();
                    std::unique_ptr<char[]> buf(new char[size]);

                    configFile.readBytes(buf.get(), size);
                    DynamicJsonDocument json(512);

                    if (DeserializationError::Ok == deserializeJson(json, buf.get())) {
                        strcpy(mqtt_server, json["mqtt_server"]);
                        strcpy(username, json["username"]);
                        strcpy(password, json["password"]);
                    }
                }
            }
        }
    }
} // namespace Config
