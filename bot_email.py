import smtplib
import time
import math

sender_email="email"
rec_email="email2"
password="password"

input_ = open("input.txt", "r")
input_content = input_.read()
state = open("state.txt", "r")
status = state.read()
lines = input_content.split('\n')
i = len(lines)
buffer = 0

buffer_tempo_list = []

def find_tempo(lines, i, buffer_tempo_list):
    counter = 0
    while(counter < i - 1):
        buffer_2 = -1
        exp = 0
        line = lines[counter]
        space_index = line.index(" ")
        line_len = len(line)
        number_size = line_len - space_index - 1
        total_number = 0
        
        if number_size == 1:
            number = int(line[line_len - 1])
            buffer_tempo_list.append(number)
        
        elif number_size > 1:
            while line[buffer_2] != " ":
                number = int(line[buffer_2])
                total_number += number * pow(10, exp)
                buffer_2 = buffer_2 - 1
                exp += 1
            buffer_tempo_list.append(total_number)
            
        counter += 1

def print_buffer_tempo_list(buffer_tempo_list):
    for elem in buffer_tempo_list:
        print(elem)

def email_sender(subject, msg):
    try:
        server=smtplib.SMTP("smtp.gmail.com", 587)
        server.ehlo()
        server.starttls()
        server.login(sender_email, password)
        print("Login success...")
        message="Subject: {}\n\n{}".format(subject, msg)
        server.sendmail(sender_email, rec_email, message)
        print("Email successfully sent to", rec_email)
        server.quit()
    except:
        print("Email failed to send")

find_tempo(lines, i, buffer_tempo_list)
print_buffer_tempo_list(buffer_tempo_list)

buffer_3 = 0

while buffer_3 < len(buffer_tempo_list):
    msg = lines[buffer_3]
    total_hours = open("total_hours.txt", "r")
    hours = int(total_hours.read())
    subject = hours
    email_sender(subject, msg)
    total_hours.close()
    time.sleep(buffer_tempo_list[buffer_3] * 60)
    hours += buffer_tempo_list[buffer_3]
    total_hours = open("total_hours.txt", "w")
    total_hours.write(str(hours))
    total_hours.close()
    buffer_3 += 1
